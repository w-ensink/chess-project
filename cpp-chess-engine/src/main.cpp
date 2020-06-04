
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include <string>
#include <queue>
#include "utils.h"
#include "MoveBuilder.h"


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


// handles all the different bitboards that will be sent from python and filters out
// unusable noise, like when two flipped bits occur in a single board change
// after a BB is added, it sends a message to it's listeners to signal where a pull or put has happened
class BitBoardManager
{
public:
    BitBoardManager() {}

    void addBitBoard (uint64_t board)
    {
        if (bitBoardHistory.empty())
            return bitBoardHistory.push_back (board);

        auto flippedBits = bitBoardHistory.back() ^ board;

        // more than one flipped bits in single change or no change at all
        if (std::bitset<64> {flippedBits}.count() != 1)
            return;

        if (bitBoardHistory.back() < board)
            for (auto* l : listeners)
                l->handlePut (flippedBits);

        else
            for (auto* l : listeners)
                l->handlePull (flippedBits);
    }

    void reset() noexcept
    {
        bitBoardHistory.clear();
    }

    struct Listener
    {
        virtual void handlePull (uint64_t bit) = 0;
        virtual void handlePut (uint64_t square) = 0;
    };

private:

    std::vector<Listener*> listeners;
    std::vector<uint64_t> bitBoardHistory;
};


//=============================================================================================






// Move manager manages the generation of moves
// constructing a move is a complex process, with many edge cases
// to solve this we use a callstack model where we analyze every item on the stack
// if it is impossible to make a valid move with what is on the stack, we need to
// unwind it until it is in a legal state again (i guess that is always just one pop)


// ======================================================================================

// Python stuurt staat van bord

// Waiting for Python script to send bit board
// Acting upon changes in board position
// expecting bitboard from python at address "/bit_board" formatted as string
// listening to Python on port 5001
// sending data to Purr Data via port 6001


// bitboard: u64, left bit = h8, right bit = a1

// schaakmat spel
// e2e4 e7e5 f1c4 b8c6 d1h5 g8f6 h5f7


int main (int argc, char* argv[])
{
    eon::initChessEngine();
    Uci::board.setToStartPos();

    auto whitePieces = std::bitset<64> {Uci::board.getAllPieces (WHITE)};
    print ("white pieces:", whitePieces);
    eon::attemptMove("h2h3");
    eon::attemptMove("a7a6");
    whitePieces = std::bitset<64> {Uci::board.getAllPieces (WHITE)};
    print ("white pieces:", whitePieces);


  //  auto sendingPort = 5000u;
   // auto receivingPort = 6000u;

   // auto sender = Local_OSC_Client {sendingPort};
   // auto receiver = Local_OSC_Server {receivingPort};

    //receiver.startListening ([&] (std::unique_ptr<OSC_Message> message)
    //{
    //    if (message->address == "/m")
    //    {
    //        auto move = message->getTypeAtIndex<std::string> (0);
//
    //        eon::attemptMove (move) ? sendStats (sender) : sender.sendMessage ("/move_failed", "i", 1);
    //    }
//
    //    else if (message->address == "/quit")
    //    {
    //        receiver.stopListening();
    //    }
    //});


    //while (receiver.isListening())
    //{
    //    std::this_thread::sleep_for (std::chrono::milliseconds (50));
    //}

    //std::cout << "done simulating chess game...\n";
    //std::cin.get();

    return 0;
}