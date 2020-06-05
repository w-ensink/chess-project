
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include <string>
#include <queue>
#include "utils.h"
#include "MoveBuilder.h"
#include "MoveBuilderTester.h"



//=============================================================================================


auto simulateChessGame (std::initializer_list<std::string> moves)
{
    eon::initChessEngine();
    Uci::board.setToStartPos();

    for (auto& m : moves)
    {
        if (! eon::attemptMove (m))
            std::cout << "Move failed: " << m << "\n";
        else
            std::cout << "Move Succeeded: " << m << " CheckMate: "
                      << std::boolalpha << eon::isCheckMate() << "\n";
    }

    eon::printBoard();
}


// schaakmat spel
// e2e4 e7e5 f1c4 b8c6 d1h5 g8f6 h5f7


int main (int argc, char* argv[])
{
    performMoveBuilderTest();

    //simulateChessGame ({"e2e4", "e7e5", "f1c4", "b8c6", "d1h5", "g8f6", "h5f7"});

    return 0;
}