
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


// schaakmat spel
// e2e4 e7e5 f1c4 b8c6 d1h5 g8f6 h5f7


int main (int argc, char* argv[])
{
    //performMoveBuilderTest();

    performChessEngineTests();
    //Local_OSC_Server server {4000};
    //Local_OSC_Client client {4000};


    //server.startListening ([](std::unique_ptr<OSC_Message> m)
    //{
    //    std::cout << "message " << m->address << '\n';
    //});
//
   //for (auto i = 0; i < 10; ++i)
   //{
   //    client.sendMessage ("/new_game", "i", 1);
   //    std::this_thread::sleep_for (std::chrono::milliseconds (50));
   //}

    //simulateChessGame ({"e2e4", "e7e5", "f1c4", "b8c6", "d1h5", "g8f6", "h5f7"});

    return 0;
}