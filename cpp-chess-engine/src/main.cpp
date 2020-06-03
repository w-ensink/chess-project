
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include <queue>
#include <string>
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



/*
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
 */

/*
0 = niks
1 = pion
2 = toren
3 = paard
4 = loper
5 = koningin
6 = koning
*/

// Returns null terminated c-string of numbers, where the numbers correspond to the pieces
// goes column by column, so first 8 chars represent the first column (or File A)
// starting from the white side (Rank 1)
auto boardToString() -> char*
{
    static char stringBuffer [65];
    memset (stringBuffer, '0', 65);
    stringBuffer[64] = '\0';

    for (auto i = 1; i < 7; ++i)
    {
        auto symbol = '0' + (char) i;

        auto piece = [] (auto index) noexcept -> int
        {
            switch (index)
            {
                case 1: return PAWN;
                case 2: return ROOK;
                case 3: return KNIGHT;
                case 4: return BISHOP;
                case 5: return QUEEN;
                case 6: return KING;
                default: return -1;
            }
        } (i);
        
        auto boardWhite = std::bitset<64> {Uci::board.getPieces (WHITE, static_cast<PieceType> (piece))};
        auto boardBlack = std::bitset<64> {Uci::board.getPieces (BLACK, static_cast<PieceType> (piece))};

        for (auto byte = 0; byte < 8; ++byte)
            for (auto bit = 0; bit < 8; ++bit)
                if (boardWhite.test (byte * 8 + bit) || boardBlack.test (byte * 8 + bit))
                    stringBuffer[bit * 8 + byte] = symbol;
    }

    return stringBuffer;
}


// sends all stats of the Uci::board to the given Local_OSC_Client
auto sendStats (Local_OSC_Client& sender)
{
    sender.sendMessage ("/pos", "s", boardToString());

    auto send = [&sender] (const std::string& colour, auto address, auto message)
    {
        auto addr = colour + address;
        sender.sendMessage (addr, "i", message);
    };

    for (auto c = 0; c < 2; ++c)
    {
        auto colour = std::string {c == 0 ? "/white" : "/black"};
        auto col = c == 0 ? WHITE : BLACK;

        auto score                  = Eval::evaluate (Uci::board, col);
        auto hasBishopPair          = Eval::hasBishopPair (Uci::board, col);
        auto numRooksOpenFiles      = Eval::rooksOnOpenFiles (Uci::board, col);
        auto mobility               = Eval::evaluateMobility (Uci::board, GamePhase::ENDGAME, col);
        auto numPassedPawns         = Eval::passedPawns (Uci::board, col);
        auto numDoubledPawns        = Eval::doubledPawns (Uci::board, col);
        auto numIsolatedPawns       = Eval::isolatedPawns (Uci::board, col);
        auto numPawnsShieldingKing  = Eval::pawnsShieldingKing (Uci::board, col);

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


int main (int argc, char* argv[])
{
    eon::initChessEngine();
    Uci::board.setToStartPos();

    auto sendingPort = 5000u;
    auto receivingPort = 6000u;

    auto sender = Local_OSC_Client {sendingPort};
    auto receiver = Local_OSC_Server {receivingPort};

    receiver.startListening ([&] (std::unique_ptr<OSC_Message> message)
    {
        if (message->address == "/m")
        {
            auto move = message->getTypeAtIndex<std::string> (0);

            eon::attemptMove (move) ? sendStats (sender) : sender.sendMessage ("/move_failed", "i", 1);
        }

        else if (message->address == "/quit")
        {
            receiver.stopListening();
        }
    });

    while (receiver.isListening())
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (50));
    }

    std::cout << "done simulating chess game...\n";
    std::cin.get();

    return 0;
}