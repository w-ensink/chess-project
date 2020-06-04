//
// Created by Wouter Ensink on 04/06/2020.
//

#include "MoveBuilderTester.h"


// =========================================================================================


MoveBuilderTester::MoveBuilderTester()
{
    initializeChessEngineDefault();
}


void MoveBuilderTester::addInstructionSet (std::string pos, std::string name, std::string move, StringList list)
{
    testInstructionSetQueue.push (std::make_unique<MoveInstructionSet> (pos, name, move, list));
}


void MoveBuilderTester::initializeChessEngine (std::string& startFen)
{
    eon::initWithFEN (startFen);
}

void MoveBuilderTester::initializeChessEngineDefault()
{
    eon::initChessEngine();
    Uci::board.setToStartPos();
    moveBuilder.setListener (this);
}

void MoveBuilderTester::illegalMoveBuildingState()
{
    std::cout << "Illegal move building state\n";
}

void MoveBuilderTester::startTestCase (MoveInstructionSet& test)
{
    // put board in start position
    initializeChessEngineDefault();
    Uci::board.setToFen (test.startPosition);
    moveBuilder.clearMove();

    // perform all puts and pulls to simulate person changing the board
    while (test.hasMoreTest())
    {
        auto instruction = test.getNextMoveInstruction();

        if (instruction.pull)
            moveBuilder.pullPiece (instruction.position);
        else
            moveBuilder.putPiece (instruction.position);
    }

    // simulate press of the commit/ply button
    auto validMove = moveBuilder.stateCanConstructValidMove();
    auto result = moveBuilder.getConstructedMove();
    auto recovery = moveBuilder.needsRecovery();

    std::cout << "\n======================== START TEST ========================\n\n";

    // if the result was as expected
    if (result == test.expectedResult)
    {

        std::cout << "Test succeeded: " << test.testName << ": " << test.expectedResult << "\n\n";
    }
    else
    {
        std::cout << "Test failed: " << test.testName << ": expected: " << test.expectedResult
                  << " but got: " << result << "\n\n";
    }

    printMoveBuilderState();

    std::cout << "\n======================== END TEST ========================\n";
}


void MoveBuilderTester::performAllTests()
{
    while (! testInstructionSetQueue.empty())
    {
        auto testCase = std::move (testInstructionSetQueue.front());
        testInstructionSetQueue.pop();

        startTestCase (*testCase.get());
    }
}


void MoveBuilderTester::printMoveBuilderState()
{
    auto validMove = moveBuilder.stateCanConstructValidMove();
    auto recovery = moveBuilder.needsRecovery();

    std::cout << "MoveBuilderState:\n";
    std::cout << "\trecovery:\t\t"    << std::boolalpha << recovery  << '\n';
    std::cout << "\tconstructable:\t" << std::boolalpha << validMove << '\n';
    std::cout << "\tpullI:\t\t\t"     << getSquareForBit (moveBuilder.pullI) << '\n';
    std::cout << "\tpullA:\t\t\t"     << getSquareForBit (moveBuilder.pullA) << '\n';
    std::cout << "\tputs[0]:\t\t"     << getSquareForBit (moveBuilder.puts[0]) << '\n';
    std::cout << "\tputs[1]:\t\t"     << getSquareForBit (moveBuilder.puts[1]) << '\n';
}