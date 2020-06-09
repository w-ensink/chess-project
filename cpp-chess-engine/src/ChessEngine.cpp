//
// Created by Wouter Ensink on 05/06/2020.
//

#include "ChessEngine.h"
#include <cstring>

struct ChessEngine::ScoreManager
{
    ScoreManager() = default;

    // returns whether the last move was bad (0), neutral (1) or good (2) for the player
    // that made the move and updates the internal score
    int getScoreInfluence()
    {
        // since the move is already done, sides have switched,
        // so the player that made the move is now the inactive player
        auto player   = Uci::board.getInactivePlayer();
        auto newScore = eon::getCurrentScoreForWhite();

        // if the player is white, a negative score difference is actually negative
        if (player == WHITE)
        {
            auto result = newScore == score ? 1 : (newScore > score ? 2 : 0);
            score = newScore;
            return result;
        }

        // if the player is black, a negative score difference is actually positive
        auto result = newScore == score ? 1 : (newScore > score ? 0 : 2);
        score = newScore;
        return result;
    }

    // in the shallow-blue engine the score for black is always the score of white
    // but negative, so we only need one variable to safe the previous score
    int score {0};
};


ChessEngine::ChessEngine (uint32_t receivingPort, uint32_t sendingPort, const char* startPos)
    : scoreManager {std::make_unique<ScoreManager>()},
      oscSender {sendingPort},
      oscReceiver {receivingPort},
      startPosition {startPos}

{
    moveBuilder.setListener (this);
    startNewGame (startPos);
    initReceiver();
}


ChessEngine::~ChessEngine()
{
    oscReceiver.stopListening();
}


[[nodiscard]] char* ChessEngine::boardToString()
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


void ChessEngine::startNewGame (const std::string& startPos)
{
    moveBuilder.clearMove();
    eon::initChessEngine();
    //Uci::board.setToStartPos();
    Uci::board.setToFen (startPos);
    oscSender.sendMessage ("/new_game", "i", 1);
}


void ChessEngine::commitButtonPressed()
{
    // if a move can be constructed
    if (moveBuilder.stateCanConstructValidMove())
    {
        auto move = moveBuilder.getConstructedMove();

        if (eon::attemptMove (move))
        {
            // move successful, game moved on, clear move builder and send stats to Purr Data
            moveBuilder.clearMove();
            oscSender.sendMessage ("/move_result", "i", scoreManager->getScoreInfluence());
            sendOSC_Messages();
        }
        else
        {
            // generated move was not a valid one, don't change move building state
            // just don't accept it in it's current form and send a osc message to Purr Data
            oscSender.sendMessage ("/move_failed", "i", 1);
        }
    }
    else
    {
        // a move construction failure, send message to Purr Data, but don't do anything else
        oscSender.sendMessage ("/move_building_error", "i", 1);
    }
}


void ChessEngine::sendOSC_Messages()
{
    oscSender.sendMessage ("/pos", "s", boardToString());

    if (eon::isCheckMate())
        oscSender.sendMessage ("/check_mate", "i", 1);

    auto send = [this] (const std::string& colour, auto address, auto message)
    {
        auto addr = colour + address;
        oscSender.sendMessage (addr, "i", message);
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


void ChessEngine::initReceiver()
{
    oscReceiver.startListening ([this] (std::unique_ptr<OSC_Message> message)
    {
        std::cout << "Engine Received: " << message->address;

        if (message->address == "/put" || message->address == "/pull")
        {
            auto squareIndex = message->getTypeAtIndex<long> (0);
            auto squareName  = getSquareForBit (boardIndexToUInt64 (squareIndex));
            std::cout << " " << squareName;
        }

        std::cout << std::endl;
        handleReceivedOSC_Message (std::move (message));
    });

    std::this_thread::sleep_for (std::chrono::milliseconds (50));
}


void ChessEngine::handleReceivedOSC_Message (std::unique_ptr<OSC_Message> message)
{
    auto address = message->address;

    if (address == "/put")
        moveBuilder.putPiece (boardIndexToUInt64 (message->getTypeAtIndex<long> (0)));

    else if (address == "/pull")
        moveBuilder.pullPiece (boardIndexToUInt64 (message->getTypeAtIndex<long> (0)));

    else if (address == "/commit")
        commitButtonPressed();

    else if (address == "/new_game")
        startNewGame (startPosition);
}


uint64_t ChessEngine::boardIndexToUInt64 (int index)
{
    return 0x1ull << static_cast<uint64_t> (63 - index);
}


void ChessEngine::illegalMoveBuildingState()
{
    // a move construction failure, send message to Purr Data, but don't do anything else
    oscSender.sendMessage ("/move_building_error", "i", 1);
}



