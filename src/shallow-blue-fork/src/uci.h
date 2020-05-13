#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "move.h"
#include "search.h"
#include "option.h"
#include "book.h"
#include <sstream>
#include <fstream>
#include <memory>


/**
 * @brief Class for handling UCI input/output.
 */
namespace Uci {
/**
 * @brief Initializes UCI options with their default values
 */
void init();

/**
 * @brief Starts listening for UCI input and responds to it when received
 */
void start();

// hacked in the forward declaration of the internal functions to make an interface

extern Board board;
extern std::vector<ZKey> positionHistory;

void uciNewGame();
void setPosition(std::istringstream &is);
void pickBestMove();
void goDepth(int depth);
void go(std::istringstream &is);
unsigned long long perft(const Board &board, int depth);


};

#endif
