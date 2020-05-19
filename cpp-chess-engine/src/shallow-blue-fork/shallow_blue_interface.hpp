
#include "src/uci.h"
#include "src/attacks.h"
#include "src/movepicker.h"
#include "src/eval.h"
#include "src/rays.h"

namespace eon
{
    void initChessEngine()
    {
        Rays::init();
        PSquareTable::init();
        ZKey::init();
        MovePicker::init();
        Attacks::init();
        Eval::init();
        Uci::init();
    }

    int runShallowBlueOriginal()
    {
        initChessEngine();
        Uci::start();

        return 0;
    }

    bool isValidMove (const std::string &moveToCheck)
    {
        MoveGen moveGenerator (Uci::board);

        for (auto move : moveGenerator.getMoves())
            if (move.getNotation() == moveToCheck)
                return true;

        return false;
    }

    bool attemptMove (const std::string& moveToAttempt)
    {
        MoveGen moveGenerator (Uci::board);

        for (auto move : moveGenerator.getMoves())
        {
            if (move.getNotation() == moveToAttempt)
            {
                Uci::board.doMove (move);
                Uci::positionHistory.push_back (Uci::board.getZKey());
                return true;
            }
        }

        return false;
    }

    void initWithFEN (const std::string& fen)
    {
        Uci::uciNewGame();
        Uci::board.setToFen (fen);
    }

    void printBoard()
    {
        std::cout << Uci::board.getStringRep() << '\n';
    }

    void goToDepth (int depth)
    {
        Uci::goDepth (depth);
    }

    MoveList getMovesForCurrentPosition()
    {
        return MoveGen (Uci::board).getMoves();
    }

    int getCurrentScoreForWhite()
    {
        return Eval::evaluate (Uci::board, WHITE);
    }

} // namespace eon