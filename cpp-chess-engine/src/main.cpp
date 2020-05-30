
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include <queue>
#include "utils.h"


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


auto sendStats(Local_OSC_Client& sender)
{
    auto send = [&sender] (const std::string& colour, auto address, auto message)
    {
        auto addr = colour + address;
        sender.sendMessage (addr, "i", message);
    };

    for (auto c = 0; c < 2; ++c)
    {
        auto colour = std::string {c == 0 ? "/white" : "/black"};

        auto score                  = Eval::evaluate (Uci::board, c == 0 ? WHITE : BLACK);
        auto hasBishopPair          = Eval::hasBishopPair (Uci::board, c == 0 ? WHITE : BLACK);
        auto numRooksOpenFiles      = Eval::rooksOnOpenFiles (Uci::board, c == 0 ? WHITE : BLACK);
        auto mobility               = Eval::evaluateMobility (Uci::board, GamePhase::ENDGAME, c == 0 ? WHITE : BLACK);
        auto numPassedPawns         = Eval::passedPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numDoubledPawns        = Eval::doubledPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numIsolatedPawns       = Eval::isolatedPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numPawnsShieldingKing  = Eval::pawnsShieldingKing (Uci::board, c == 0 ? WHITE : BLACK);

        send (colour, "/score", score);
        send (colour, "/has_bishop_pair", (int) hasBishopPair);
        send (colour, "/num_rooks_open_files", numRooksOpenFiles);
        send (colour, "/mobility", mobility);
        send (colour, "/num_passed_pawns", numPassedPawns);
        send (colour, "/num_doubled_pawns", numDoubledPawns);
        send (colour, "/num_isolated_pawns", numIsolatedPawns);
        send (colour, "/num_pawns_shielding_king", numPawnsShieldingKing);
    }
}


auto simulateChessGame (Local_OSC_Client& sender, const std::vector<std::string_view>& moves)
{
    std::cout << "OSC Sending to port 5000\n";

    eon::initChessEngine();
    Uci::board.setToStartPos();

    for (auto move : moves)
    {
        if (! eon::attemptMove (move))
            return false;

        sendStats (sender);

        sleep (rand() % 5 + 1);
    }

    return true;
}

int main (int argc, char* argv[])
{

    eon::initChessEngine();
    Uci::board.setToStartPos();

    auto sendingPort = 5000u;
    auto receivingPort = 6000u;

    auto sender = Local_OSC_Client {sendingPort};
    auto receiver = Local_OSC_Server {receivingPort};

    std::queue<std::unique_ptr<OSC_Message>> messageQueue;

    receiver.startListening ([&] (std::unique_ptr<OSC_Message> message)
    {
        if (message->address == "/m")
        {
            auto move = message->getTypeAtIndex<std::string> (0);

            if (! eon::attemptMove (move))
                sender.sendMessage ("/failed_move", "i", 1);
            else
                sendStats (sender);

        }
        else if (message->address == "/quit")
            receiver.stopListening();
    });


    //simulateChessGame (sender, {"e2e4", "d7d5", "c2c3", "b8a6"});

    while (receiver.isListening())
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (50));
    }

    std::cout << "done simulating chess game...\n";
    std::cin.get();

    return 0;
}