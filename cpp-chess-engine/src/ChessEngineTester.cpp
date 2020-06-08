//
// Created by Wouter Ensink on 05/06/2020.
//

#include "ChessEngineTester.h"


ChessEngineTester::ChessEngineTester()
{
    chessEngine.startNewGame (startFEN);

    statusReceiver.startListening ([] (std::unique_ptr<OSC_Message> message)
    {

        //std::cout << "Tester Received: " << message->address << " " << message->getIndexAsString (0) << "\n";
    });
}


ChessEngineTester::~ChessEngineTester()
{
    statusReceiver.stopListening();
}


bool ChessEngineTester::attemptMoveOnBoard (const std::string& move, Board& board)
{
    for (auto m : MoveGen (board).getLegalMoves())
    {
        if (m.getNotation() == move)
        {
            board.doMove (m);
            return true;
        }
    }

    return false;
}


bool ChessEngineTester::areBoardsEqual (Board& a, Board& b)
{
    return a.getStringRep() == b.getStringRep();
}

void ChessEngineTester::addTest (const EngineInstructionTestCase& caseToAdd)
{
    testQueue.push (caseToAdd);
}

void ChessEngineTester::runAllTests()
{
    while (! testQueue.empty())
    {
        runTest (testQueue.front());
        testQueue.pop();
    }
}

void ChessEngineTester::runTest (const EngineInstructionTestCase& test)
{
    std::cout << "Start of TestCase: " << test.name << "\n\n";

    for (auto message : test.oscMessages)
    {
        std::cout << "sending: " << message.first << " " << message.second << '\n';
        commandSender.sendMessage (message.first, "i", message.second);
    }

    // wait 50 milliseconds for all messages to be handled..
    std::this_thread::sleep_for (std::chrono::milliseconds (50));

    auto referenceBoard = Board{};

    for (auto& move : test.expectedOutcome)
        attemptMoveOnBoard (move, referenceBoard);

    if (areBoardsEqual (referenceBoard, Uci::board))
    {
        std::cout << "Test Succeeded: " << test.name << "\n";
    }
    else
    {
        std::cout << "Test Failed: " << test.name << "\n";

        std::cout << "=============== EXPECTED BOARD ================\n\n"
                  << referenceBoard.getStringRep()
                  << "\n=============== TEST RESULT BOARD ================\n\n"
                  << Uci::board.getStringRep() << "\n\n";
    }
}

