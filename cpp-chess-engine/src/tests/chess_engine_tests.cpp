//
// Created by Wouter Ensink on 13/05/2020.
//

#include "../tinyosc/tinyosc.h"
#include "../shallow-blue-fork/shallow_blue_interface.hpp"
#include "../utils.h"
#include "../shallow-blue-fork/src/generalmovepicker.h"
#include <vector>
#include <map>
#include "../shallow-blue-fork/src/eval.h"

struct TestPosition {};

auto testMoveValidation()
{
    std::map<std::string, bool> moveOptions
    {
        {"e2e4", true},
        {"e2e5", false},
        {"b1c3", true},
        {"e7e5", false}
    };

    for (auto& move : moveOptions)
    {
        if (eon::isValidMove (move.first) != move.second)
        {
            print("Move Validation Test Failed");
            return false;
        }
    }

    return true;
}

#define testFEN "rnbqkbnr/p1p1pppp/1p1p4/2P5/8/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1"
#define LUCENA "1K6/1P1k4/8/8/2R5/8/8/1r6 w - - 0 1"
//==================================================================================
#define CAP_QUEEN "rnb1kbnr/p1p1pppp/1p1p4/2P1q3/3P4/8/PP2PPPP/RNBQKBNR b KQkq - 0 1"

#define CHECK "rnb1kbnr/p1p1pppp/1p1pq3/2P5/3PK3/8/PP2PPPP/RNBQ1BNR w kq - 0 1"

#define PROMOTION "r3kbnr/pPp1pppp/3p4/2P1q3/3P4/8/P3PPPP/RNBQKBNR w KQkq - 0 1"

auto testTinyOsc()
{

}



//==================================================================================

std::vector<std::string> offsetMoves {
    ""
};

auto testNext (const Board& board)
{
    std::string best ={};
    TranspTable transpTable {};
    OrderingInfo orderingInfo {&transpTable};
    auto moves = MoveGen(board).getMoves();
    GeneralMovePicker picker {&orderingInfo, &board, &moves};

    while (picker.hasNext())
    {
        auto m = picker.getNext().getValue();

    }
}

int main()
{
    eon::initChessEngine();
    //Uci::board.setToStartPos();
    Uci::board.setToFen (PROMOTION);
    if (eon::attemptMove ("b7a8q")) print ("moved b7a8q");
    eon::printBoard();

    auto moves = eon::getMovesForCurrentPosition();
    TranspTable transpTable {};
    OrderingInfo orderingInfo {&transpTable};
    GeneralMovePicker picker {&orderingInfo, &Uci::board, &moves};

    int n= 1;
    while (picker.hasNext())
    {
        auto move = picker.getNext();
        print("move",n++,":", move.getNotation(), "value:", move.getValue(), "capture:", move.getCapturedPieceType());
    }

    //eon::goToDepth(3);

    print ("score for white:", Eval::evaluate(Uci::board, WHITE));
    print ("score for black:", Eval::evaluate(Uci::board, BLACK));

   // std::cin.get();

    return 0;
}