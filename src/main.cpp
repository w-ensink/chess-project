
/** Written by Wouter Ensink */



#include "shallow_blue_interface.hpp"
#include "utils.h"


int main (int argc, char* argv[])
{
    //runShallowBlueOriginal();
    eon::initChessEngine();

    Uci::board.setToStartPos();

    if (eon::isValidMove("e2e4")) {
        print("e2e4 is valid");
    }

    if (! eon::isValidMove("e2e5")) {
        print("e2e5 is not a valid move");
    }

    Uci::goDepth(3);

    std::cin.get();
    print("done");

    return 0;
}


