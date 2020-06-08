
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <osc_interface.h>
#include <string>
#include <queue>
#include "utils.h"
#include "MoveBuilder.h"
#include "MoveBuilderTester.h"
#include "ChessEngineTester.h"


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


int main (int argc, char* argv[])
{
    auto* startPosition = "nkqr4/pppp4/8/8/8/8/PPPP4/RKQN4 w Q - 0 1";
    auto sendingPort = 5001u;
    auto receivingPort = 6001u;
    print ("Chess Engine will send on:", sendingPort, "and receive on:", receivingPort);
    auto engine = ChessEngine {receivingPort, sendingPort, startPosition};

    print ("Press enter to stop engine");
    std::cin.get();

    return 0;
}