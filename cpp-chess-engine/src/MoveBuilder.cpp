//
// Created by Wouter Ensink on 04/06/2020.
//

#include "MoveBuilder.h"


// puts can undo pulls only if they're done in the right order
void MoveBuilder::handlePutForRecovery (uint64_t pos)
{
    // it's either not put back in the correct order, or it's a new mistake
    if (pullsToUndo.top() != pos)
        putsToUndo.push (pos);

    // else it is the top of the stack, so the undo is handled by popping it off
    else
        pullsToUndo.pop();

    update();
}


// pulls should always be put back in the opposite direction
void MoveBuilder::handlePullForRecovery (uint64_t pos)
{
    // it's either not pulled in the right order, or it's a new mistake
    if (putsToUndo.top() != pos)
        pullsToUndo.push (pos);
    // else it's the top of the stack, so the undo is handled by popping it off
    else
        putsToUndo.pop();

    update();
}


void MoveBuilder::putPiece (uint64_t pos)
{
    // if recovery is needed, handle the put in recovery mode
    if (needsRecovery())
        return handlePutForRecovery (pos);

    // if no piece has been pulled yet, putting one is illegal
    if (! (pullA || pullI)) { putsToUndo.push (pos); update(); return; }


    // if there is still place left in the put registers-> put it there
    if (puts[0] == 0ull) { puts[0] = pos; update(); return; }
    if (puts[1] == 0ull) { puts[1] = pos; update(); return; }

    // else it is an illegal put (too many puts), and it should be undone
    putsToUndo.push (pos);
    update();
}


void MoveBuilder::pullPiece (uint64_t pos)
{
    // if recovery is needed, handle the pull in recovery mode
    if (needsRecovery())
        return handlePullForRecovery (pos);

    // square we pull from contained an active player -> put it in the pullA register
    if (squareContainsActivePlayer (pos))
    {
        // if pullA is taken -> it's an illegal pull (more than one pullA)
        if (pullA)
            pullsToUndo.push (pos);
        else
            pullA = pos;
    }

    // if the square contained an inactive player:
    //  - the puts contain this position(so an active piece could have been put on it)
    //  - the puts don't contain this pos -> inactive is pulled
    else if (squareContainsInactivePlayer (pos))
    {
        // if pullI was set and pullA too and a put was done on this square
        //  - probably it's an undo of that put
        // e.g.: pull(e2) pull(d3) put(d3) pull(d3) // e2==white && d3==black
        if (pullI && pullA && putsContains (pos))
            removeFromPuts (pos);

            // any other scenario where pullI was already set: illegal move
            // e.g.: pull(d3) pull(a8)  // d3==black && a8==black
        else if (pullI)
            pullsToUndo.push (pos);

            // if pullI was not set: it should now be set
            // e.g.: pull(a8) // a8==black
        else
            pullI = pos;
    }
    // if during a move a piece was (temporarily) put on an empty square
    else if (putsContains (pos))
    {
        // remove it from puts
        removeFromPuts (pos);
    }
    else
    {
        std::cout << "Error: pulling piece from empty square: " << "\n";
        return;
    }

    update();
}


// handles cleaning and simplifying of the current state
void MoveBuilder::update()
{
    // if there are two pairs of matching pull & puts -> assume both have been put back
    // e.g.: pull(a1) pull(a8) put(a8) put(a1) // a1==white && a8==black
    if (putsContains (pullA) && putsContains (pullI))
        clearMove();

    // if the pullA matches with a put, but the pullI doesn't
    // assume active piece was put back and the user will do something else
    // e.g.: pull(a1) pull(a8) put(a1) // a1==white && a8==black
    // e.g.: pull(a1) put(a1)
    if (putsContains (pullA) && ! putsContains (pullI))
    {
        removeFromPuts (pullA);
        pullA = 0ull;
    }

    // notify listeners if recovery mode is active
    if (needsRecovery() && listener != nullptr)
        listener->illegalMoveBuildingState();
}


// returns whether a move can be constructed from the current state
// does not check if that move is possible in the current game position
[[nodiscard]] bool MoveBuilder::stateCanConstructValidMove() const
{
    // without picking up one of your pieces, you can't make a move..
    // in recovery mode it's not legal to make a move
    if (! pullA || needsRecovery()) return false;

    // when you pick up your own, pick up the piece to capture, then put your own on the
    // square of the piece you just captured
    if (pullI && putsContains (pullI) && ! putsContains (pullA))
        return true;

    // when you pick up your own and put it on an empty square
    return ! pullI && ! putsContains (pullA) && ! putsEmpty();
}


// returns a move in a1b2 string format if one can be constructed
[[nodiscard]] std::string MoveBuilder::getConstructedMove()
{
    if (! stateCanConstructValidMove())
        return "invalid";

    // now we assume a move can be constructed: it kinda means that
    // - if pullI was set: from pullA to pullI
    // - if only pullA was set: from pullA to the one put
    if (pullI)
    {
        return getSquareForBit (pullA) + getSquareForBit (pullI);
    }

    return getSquareForBit (pullA) + getSquareForBit (getOnePutSquare());
}


// checks if the given position matches with any of the puts
// returns false if a 0 position was given
[[nodiscard]] bool MoveBuilder::putsContains (uint64_t pos) const
{
    if (! pos)
        return false;

    return puts[0] == pos || puts[1] == pos;
}


// returns whether both ints in puts are zero (so not set)
[[nodiscard]] bool MoveBuilder::putsEmpty() const
{
    return puts[0] == 0ull && puts[1] == 0ull;
}


// removes the given position from puts, if it contains it
void MoveBuilder::removeFromPuts (uint64_t pos)
{
    if (puts[0] == pos) puts[0] = 0ull;
    if (puts[1] == pos) puts[1] = 0ull;
}


// removes all info about the move that was under construction, essentially starting a new move
void MoveBuilder::clearMove()
{
    pullA = 0ull;
    pullI = 0ull;
    puts[0] = 0ull;
    puts[1] = 0ull;

    // clear undo stacks
    while (! putsToUndo.empty())
        putsToUndo.pop();

    while (! pullsToUndo.empty())
        pullsToUndo.pop();
}


// if there are steps to undo, the recovery is active
[[nodiscard]] bool MoveBuilder::needsRecovery() const
{
    return ! (pullsToUndo.empty() && putsToUndo.empty());
}


[[nodiscard]] int MoveBuilder::countPuts() const
{
    if (putsEmpty()) return 0;
    if (puts[0] != 0ull && puts[1] != 0ull) return 2;
    return 1;
}


[[nodiscard]] uint64_t MoveBuilder::getOnePutSquare()
{
    return puts[0] ? puts[0] : puts[1];
}

void MoveBuilder::setListener (MoveBuilder::Listener *l)
{
    listener = l;
}
