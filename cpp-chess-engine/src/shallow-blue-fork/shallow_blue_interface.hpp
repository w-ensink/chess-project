
#ifndef __SHALLOW_BLUE_INTERFACE__
#define __SHALLOW_BLUE_INTERFACE__

#include "src/uci.h"
#include "src/attacks.h"
#include "src/movepicker.h"
#include "src/eval.h"
#include "src/rays.h"

namespace eon
{
    inline void initChessEngine()
    {
        Rays::init();
        PSquareTable::init();
        ZKey::init();
        MovePicker::init();
        Attacks::init();
        Eval::init();
        Uci::init();
    }

    inline int runShallowBlueOriginal()
    {
        initChessEngine();
        Uci::start();

        return 0;
    }

    inline bool isValidMove (const std::string &moveToCheck)
    {
        MoveGen moveGenerator (Uci::board);

        for (auto move : moveGenerator.getMoves())
            if (move.getNotation() == moveToCheck)
                return true;

        return false;
    }

    inline bool attemptMove (std::string_view moveToAttempt)
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

    // check mate if no moves are possible
    inline bool isCheckMate()
    {
        return MoveGen (Uci::board).getLegalMoves().size() == 0;
    }

    inline void initWithFEN (const std::string& fen)
    {
        Uci::uciNewGame();
        Uci::board.setToFen (fen);
    }

    inline void printBoard()
    {
        std::cout << Uci::board.getStringRep() << '\n';
    }

    inline void goToDepth (int depth)
    {
        Uci::goDepth (depth);
    }

    inline MoveList getMovesForCurrentPosition()
    {
        return MoveGen (Uci::board).getMoves();
    }

    inline int getCurrentScoreForWhite()
    {
        return Eval::evaluate (Uci::board, WHITE);
    }

} // namespace eon

#endif