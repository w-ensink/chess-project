//
// Created by Wouter Ensink on 05/06/2020.
//

#ifndef CHESS_ENGINE_CHESSENGINE_H
#define CHESS_ENGINE_CHESSENGINE_H

#include <osc_interface.h>
#include <cstdint>
#include <memory>
#include <thread>
#include <chrono>
#include <bitset>
#include <string>

#include "MoveBuilder.h"



// Manages the input from Python and sends outgoing stuff to Purr Data
// ------------------------------------------------------------------------------------------------
// Expected incoming OSC addresses:
//  - '/new_game'                   [1]             (will start new match, deleting all current state)
//  - '/put'                        [square_index]  (will act as if a piece was put on that square)
//  - '/pull'                       [square_index]  (will act as if a piece was lifted on that square)
//  - '/commit'                     [1]             (will attempt to make a move based on previous puts/pulls)
// ------------------------------------------------------------------------------------------------
// Produces outgoing OSC messages:
//  - '/pos'                        [pos_string]    (send string of 64 numbers, see boardToString for more info)
//  - '/new_game'                   [1]             (will be sent when a new game begins)
//  - '/move_building_error'        [1]             (will be sent when a move construction error occurs)
//  - '/move_failed'                [1]             (will be sent when a move wasn't valid in current pos)
//  - '/colour'                     [is_white]      (will send 1 if active colour is white, else 0)
//  - '/check_mate'                 [1]             (will be sent when someone is check mate)
//  - '/move_result'                [0|1|2]         (sends whether move was bad(0), neutral(1) or good(2))
//
// the following addressed go separately for black and white (e.g.: '/white/score' & '/black/score'):
//  - '/score'                      [num]           (sends score)
//  - '/has_bishop_pair             [num]           (send whether both bishops are still in the game)
//  - '/num_rooks_open_files'       [num]           (send number of rooks on open files for colour)
//  - '/mobility'                   [num]           (send mobility score)
//  - '/num_passed_pawns'           [num]           (send number of passed pawns for colour)
//  - '/num_doubled_pawns'          [num]           (send number of doubled pawns for colour)
//  - '/num_isolated_pawns'         [num]           (send number of isolated pawns for colour)
//  - '/num_pawns_shielding_king'   [num]           (send number of pawns shielding king for colour)
// ------------------------------------------------------------------------------------------------

const auto startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class ChessEngine   : private MoveBuilder::Listener
{
public:

    // sets up osc connections with given ports
    explicit ChessEngine (uint32_t receivingPort = 6000,
                          uint32_t sendingPort = 5000,
                          const char* startPos = startFEN);

    // closes the Local_OSC_Server (receiver)
    ~ChessEngine();

    // start a new chess match, clears board to start pos, clears move builder
    // also notifies via sender on address '/new_game'
    void startNewGame (const std::string& startPos);



private:

    // internal struct to handle score changes, to figure out if a move was good or bad
    struct ScoreManager;

    std::unique_ptr<ScoreManager> scoreManager;
    MoveBuilder moveBuilder;
    Local_OSC_Client oscSender;
    Local_OSC_Server oscReceiver;


    // function from base class MoveBuilder::Listener
    void illegalMoveBuildingState() override;

    // called when the commit-move button was pressed on the physical board
    void commitButtonPressed();

    // will send all OSC messages to its oscSender
    void sendOSC_Messages();

    // give the receiver it's callback, should be called only once, in constructor
    void initReceiver();

    // handles the incoming OSC_Messages (bare in mind this is not handled on the main thread)
    void handleReceivedOSC_Message (std::unique_ptr<OSC_Message> message);


    // takes index of board square: a1 == 63, g8 == 1
    // returns 64 bit int representation of the same thing
    [[nodiscard]] static uint64_t boardIndexToUInt64 (int index);

    // Returns null terminated c-string of numbers, where the numbers correspond to the pieces
    // goes column by column, so first 8 chars represent the first column (or File A)
    // starting from the white side (Rank 1)
    // 0 = empty
    // 1 = pawn
    // 2 = rook
    // 3 = knight
    // 4 = bishop
    // 5 = queen
    // 6 = king
    [[nodiscard]] static char* boardToString();

    const std::string startPosition;
};


#endif //CHESS_ENGINE_CHESSENGINE_H
