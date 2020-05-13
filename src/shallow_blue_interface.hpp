
#include "shallow-blue-fork/src/uci.h"
#include "shallow-blue-fork/src/attacks.h"
#include "shallow-blue-fork/src/movepicker.h"
#include "shallow-blue-fork/src/eval.h"
#include "shallow-blue-fork/src/rays.h"

namespace eon
{
    auto initChessEngine()
    {
        Rays::init();
        PSquareTable::init();
        ZKey::init();
        MovePicker::init();
        Attacks::init();
        Eval::init();
        Uci::init();
    }

    auto runShallowBlueOriginal()
    {
        initChessEngine();
        Uci::start();

        return 0;
    }

    bool isValidMove (const std::string &moveToCheck)
    {
        MoveGen movegen (Uci::board);

        for (auto move : movegen.getMoves())
            if (move.getNotation() == moveToCheck)
                return true;

        return false;
    }

    bool attemptMove (const std::string& moveToAttempt)
    {
        MoveGen movegen (Uci::board);

        for (auto move : movegen.getMoves()) {
            if (move.getNotation() == moveToAttempt) {
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
        std::istringstream ss {fen};
        Uci::setPosition (ss);
    }

} // namespace eon