//
// Created by Wouter Ensink on 04/06/2020.
//

#ifndef CHESS_ENGINE_MOVEBUILDER_H
#define CHESS_ENGINE_MOVEBUILDER_H

#include <cstdint>
#include <shallow_blue_interface.hpp>
#include <array>
#include <stack>


//=============================================================================================

[[nodiscard]] inline bool squareContainsWhitePiece (uint64_t bitBoardSquare)
{
    return Uci::board.getAllPieces (WHITE) & bitBoardSquare;
}

[[nodiscard]] inline bool squareContainsBlackPiece (uint64_t bitBoardSquare)
{
    return Uci::board.getAllPieces (BLACK) & bitBoardSquare;
}

[[nodiscard]] inline bool squareContainsInactivePlayer (uint64_t bitBoardSquare)
{
    return Uci::board.getActivePlayer() == BLACK ? squareContainsWhitePiece (bitBoardSquare)
                                                 : squareContainsBlackPiece (bitBoardSquare);
}

[[nodiscard]] inline bool squareContainsActivePlayer (uint64_t bitBoardSquare)
{
    return Uci::board.getActivePlayer() == WHITE ? squareContainsWhitePiece (bitBoardSquare)
                                                 : squareContainsBlackPiece (bitBoardSquare);
}

// returns index of set bit, counted from the right
[[nodiscard]] inline constexpr  uint32_t indexOfSetBit (uint64_t word) noexcept
{
    auto index = 0u;
    while (! (word & (0x1ull << index))) ++index;
    return 63 - index;
}

// 01000000 00000000 00000000 00000000   00000000 00000000 00000000 00000000 ==> g8
// 00000000 00001000 00000000 00000000   00000000 00000000 00000000 00000000 ==> d7
// 00000000 00000000 00000000 00000000   00000000 00000000 00000000 00000001 ==> a1
[[nodiscard]] inline std::string getSquareForBit (uint64_t word)
{
    if (! word)
        return "no_square";

    auto bitIndex = indexOfSetBit (word);
    auto file = 'h' - static_cast<char> (bitIndex % 8);
    auto rank = '1' + static_cast<char> (7 - (bitIndex / 8));
    char buffer[3];
    sprintf (buffer, "%c%c", file, rank);
    buffer[2] = '\0';
    return std::string {buffer};
}

// returns bitboard with only the given square activated
// d7 ==> 00000000 00001000 00000000 00000000   00000000 00000000 00000000 00000000
[[nodiscard]] inline uint64_t getBitForSquare (const std::string& square)
{
    if (square == "no_square")
        return 0x0ull;

    auto file = static_cast<uint32_t> (square[0] - 'a');
    auto rank = static_cast<uint32_t> (square[1] - '1');
    auto bitIndex = file + rank * 8;
    return 0x1ull << bitIndex;
}

// ========================================================================================
//
// MoveBuilder attempts to construct moves based on the current state of the Uci::board
// and the given pull (taking a piece from the board) and put (placing a piece on the board) commands.
// It is self correcting, which means it will accept the taking back of a move as long
// as it still has the info of the construction (clearMove has not yet been called).
// It also has some kind of recovery mode, which will activate when it is unable to follow
// what is happening (e.g.: you pull two pieces of the same colour).
// When it enters this mode, it will send a message to it's listener and waits for the piece that
// caused the activation of the recovery mode, to be put back.
// This has to be done in the exact opposite order you picked them up, otherwise you'll just get more
// moves to undo.

class MoveBuilder
{
public:

    MoveBuilder() = default;

    // virtual base class for a class that needs to listen for the current state
    // for example to mange the interaction with the sound engine
    struct Listener
    {
        virtual void illegalMoveBuildingState() = 0;
    };

    void setListener (Listener* l);

    // should be used by the input handler to give the builder input about put pieces
    void putPiece (uint64_t pos);

    // should be used by the input handler to give the builder input about pulled pieces
    void pullPiece (uint64_t pos);

    // returns whether a move can be constructed from the current state
    // does not check if that move is possible in the current game position
    [[nodiscard]] bool stateCanConstructValidMove() const;

    // returns a move in a1b2 string format if one can be constructed
    [[nodiscard]] std::string getConstructedMove();

    // removes all info about the move that was under construction, essentially starting a new move
    void clearMove();

    // if there are steps to undo, the recovery is active
    [[nodiscard]] bool needsRecovery() const;

    friend class MoveBuilderTester;

private:

    uint64_t pullA {0};
    uint64_t pullI {0};
    std::array<uint64_t, 2> puts {0,0};

    // undo's should be formatted in a stack.. steps should be undone in the opposite order
    std::stack<uint64_t> pullsToUndo; // contains the place where something needs to be put before advancing
    std::stack<uint64_t> putsToUndo;  // if a put is done while illegal, the system should wait for a pull from that pos
    Listener* listener {nullptr};


    // handles cleaning and simplifying of the current state
    void update();

    // puts can undo pulls only if they're done in the right order
    void handlePutForRecovery (uint64_t pos);

    // pulls should always be put back in the opposite direction
    void handlePullForRecovery (uint64_t pos);

    // removes the given position from puts, if it contains it
    void removeFromPuts (uint64_t pos);

    // returns number of valid non null puts
    [[nodiscard]] int countPuts() const;

    // gets the square with the destination, assuming only one put is active
    [[nodiscard]] uint64_t getOnePutSquare();

    // checks if the given position matches with any of the puts
    // returns false if a 0 position was given
    [[nodiscard]] bool putsContains (uint64_t pos) const;

    // returns whether both ints in puts are zero (so not set)
    [[nodiscard]] bool putsEmpty() const;

};

#endif //CHESS_ENGINE_MOVEBUILDER_H
