//
// Created by Wouter Ensink on 05/06/2020.
//

#ifndef CHESS_ENGINE_CHESSENGINETESTER_H
#define CHESS_ENGINE_CHESSENGINETESTER_H

#include <queue>
#include "ChessEngine.h"


// a1 == bitBoard 0x1 == index 63
inline long u64ToBoardIndex (uint64_t bitBoard)
{
    long index = 0;
    while (! (bitBoard & 0x8000'0000'0000'0000ull))
    {
        bitBoard <<= 0x1ull;
        ++index;
    }

    return index;
}

// translate square "a1" to board index 63 for example
inline long squareToBoardIndex (const std::string& square)
{
    return u64ToBoardIndex (getBitForSquare (square));
}


struct EngineInstructionTestCase
{
    std::string name;
    // messages can be "/put" "/pull" "/commit" & "/new_game"
    // each case should start with a "/new_game"
    std::vector<std::pair<std::string, int>> oscMessages;
    std::vector<std::string> expectedOutcome;
};


// Test engine for the ChessEngine
// Goals:
//  - simulate put, pull, commit & new_game command
//  - simulate them both in normal, predictable order, as well as some fuzzing
//

class ChessEngineTester
{
public:
    ChessEngineTester();

    ~ChessEngineTester();

    void addTest (const EngineInstructionTestCase&);

    void runAllTests();

private:

    ChessEngine chessEngine {6001, 5001};
    Local_OSC_Server statusReceiver {5001};
    Local_OSC_Client commandSender {6001};
    std::queue<EngineInstructionTestCase> testQueue;


    void runTest (const EngineInstructionTestCase&);

    // attempts given move on given board position, returns whether it succeeded
    [[nodiscard]] static bool attemptMoveOnBoard (std::string move, Board& board);

    // Checks if both boards are the same based on their string representation
    [[nodiscard]] static bool areBoardsEqual (Board& a, Board& b);
};


// function to invoke to run all tests for the ChessEngine
inline void performChessEngineTests()
{
    auto simpleTestCase = EngineInstructionTestCase
    {
        "simple_single_move",
        {
                {"/new_game", 1},
                {"/pull", squareToBoardIndex ("a2")},
                {"/put", squareToBoardIndex ("a4")},
                {"/commit", 1}
        },
        {"a2a4"}
    };

    // pull(a2) pull(e2) put(e2) put(a4) commit
    // pull(b7) put(b6) pull(b6) put(b5) commit
    // should be a2a4 - b7b5
    auto twoMoveDoubtCase = EngineInstructionTestCase
    {
        "two_move_doubt",
        {
                {"/new_game", 1},
                {"/pull", squareToBoardIndex("a2")},
                {"/pull", squareToBoardIndex("e2")}, // enter recovery
                {"/put",  squareToBoardIndex("e2")}, // exit recovery
                {"/put",  squareToBoardIndex("a4")},
                {"/commit", 1},
                {"/pull", squareToBoardIndex("b7")},
                {"/put",  squareToBoardIndex("b6")},
                {"/pull", squareToBoardIndex("b6")},
                {"/put",  squareToBoardIndex("b5")},
                {"/commit", 1}
        },
        {"a2a4", "b7b5"}
    };

    auto testEngine = ChessEngineTester{};
    testEngine.addTest (simpleTestCase);
    testEngine.addTest (twoMoveDoubtCase);

    std::cout << "START CHESS ENGINE TEST\n\n";
    testEngine.runAllTests();
}



#endif //CHESS_ENGINE_CHESSENGINETESTER_H
