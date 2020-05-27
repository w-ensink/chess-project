
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include "utils.h"
#include <juce_core/juce_core.h>
#include <juce_osc/juce_osc.h>

auto evaluateCurrentPosition()
{
    auto score                  = Eval::evaluate (Uci::board, WHITE);
    auto hasBishopPair          = Eval::hasBishopPair (Uci::board, WHITE);
    auto numRooksOpenFiles      = Eval::rooksOnOpenFiles (Uci::board, WHITE);
    auto mobility               = Eval::evaluateMobility (Uci::board, GamePhase::ENDGAME, WHITE);
    auto numPassedPawns         = Eval::passedPawns (Uci::board, WHITE);
    auto numDoubledPawns        = Eval::doubledPawns (Uci::board, WHITE);
    auto numIsolatedPawns       = Eval::isolatedPawns (Uci::board, WHITE);
    auto numPawnsShieldingKing  = Eval::pawnsShieldingKing (Uci::board, WHITE);

    print ("score:", score);
    print ("has bishop pair:", hasBishopPair);
    print ("num rooks on open file:", numRooksOpenFiles);
    print ("mobility:", mobility);
    print ("num passed pawns:", numPassedPawns);
    print ("num doubled pawns:", numDoubledPawns);
    print ("num isolated pawns:", numIsolatedPawns);
    print ("num pawns shielding king:", numPawnsShieldingKing);
}




auto simulateChessGame (const std::vector<std::string_view>& moves)
{

    eon::initChessEngine();
    Uci::board.setToStartPos();

    for (auto move : moves)
    {
        if (! eon::attemptMove (move))
            return false;

        sleep (rand() % 10);
    }

    return true;
}



int main (int argc, char* argv[])
{
    eon::initChessEngine();

    Uci::board.setToStartPos();

    if (eon::attemptMove ("e2e4"))
        print ("e2e4 is valid");


    if (! eon::attemptMove ("e2e5"))
        print ("e2e5 is not a valid move");

    eon::printBoard();
    evaluateCurrentPosition();

    return 0;
}


