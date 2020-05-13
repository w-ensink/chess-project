//
// Created by Wouter Ensink on 13/05/2020.
//

#include "../tinyosc/tinyosc.h"
#include "../shallow_blue_interface.hpp"
#include "../utils.h"
#include <vector>
#include <map>

struct TestPosition {};

auto testMoveValidation()
{
    std::map<std::string, bool> moveOptions {
        {"e2e4", true},
        {"e2e5", false},
        {"b1c3", true},
        {"e7e5", false}
    };

    for (auto& move : moveOptions)
    {
        if (eon::isValidMove(move.first) != move.second)
        {
            print("Move Validation Test Failed");
            return false;
        }
    }

    return true;
}

#define testFEN "rnbqkbnr/p1p1pppp/1p1p4/2P5/8/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1"

//==================================================================================


auto testTinyOsc()
{

}



//==================================================================================

int main()
{
    eon::initChessEngine();

    eon::printBoard();

    if (testMoveValidation()) {
        print("Move Validation Test Succeeded");
    }

    // "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    // "rnbqkbnr/p1p1pppp/1p1p4/2P5/8/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1"
    eon::initWithFEN(testFEN);

    eon::printBoard();

    if (eon::attemptMove("b6c5")) {
        print("moved b6 to c5 and captured pawn");
        eon::printBoard();
    }

    return 0;
}