//
// Created by Wouter Ensink on 04/06/2020.
//

// System to test MoveBuilder

/* Goals:
 *  - test how the MoveBuilder behaves when normal, predictable input is given
 *  - test how the MoveBuilder handles weird step combos
 *  - test if moves are correctly recognized
 *
 * */

#ifndef CHESS_ENGINE_MOVEBUILDERTESTER_H
#define CHESS_ENGINE_MOVEBUILDERTESTER_H

#include <queue>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <iostream>

#include "MoveBuilder.h"

// describes a single move instruction, e.g.: pull(e2) or put(a5)
struct MoveInstruction
{
    // true if pull, false if put..
    bool pull;
    uint64_t position;
};



// descibes a series of steps of puts and pulls
struct MoveInstructionSet
{
    using StringList = std::initializer_list<std::string>;

    // steps should be inserted as a list
    //  - pull('a1') -> "-a1"
    //  - put('a1')  -> "+a1"
    MoveInstructionSet (std::string pos, std::string name, std::string move, StringList list)
    {
        instructionStrings = list;
        startPosition = std::move (pos);
        expectedResult = std::move (move);
        testName = std::move (name);

        for (auto i : list)
        {
            auto sq = std::string {i.begin() + 1, i.end()};
            instructionQueue.push ({i[0] == '-', getBitForSquare (sq)});
        }
    }

    // MoveInstructionSet (const MoveInstructionSet& other) = default;

    // pops the next instruction from the queue and returns it
    MoveInstruction getNextMoveInstruction()
    {
        auto instr = instructionQueue.front();
        instructionQueue.pop();
        return instr;
    }

    [[nodiscard]] bool hasMoreTest() const
    {
        return ! instructionQueue.empty();
    }

    std::string startPosition;
    std::vector<std::string> instructionStrings;
    std::queue<MoveInstruction> instructionQueue;
    std::string expectedResult;
    std::string testName;
};



// Test engine for the MoveBuilder
class MoveBuilderTester   : MoveBuilder::Listener
{
public:
    using StringList = std::initializer_list<std::string>;

    MoveBuilderTester();

    // Add an instruction set to test:
    //  - pos: start position (FEN)
    //  - name: name for the test
    //  - move: move to attempt
    //  - instructions: list of put/pull instructions: pull(a1) -> "-a1", put(a1) -> "+a1"
    void addInstructionSet (std::string pos, std::string name, std::string move, StringList instructions);

    void initializeChessEngine (std::string& startFen);

    void initializeChessEngineDefault();

    // called from move builder when it's impossible to determine the move that is under construction
    void illegalMoveBuildingState() override;

    void performAllTests();

private:


    // start the test for the given test case
    void startTestCase (MoveInstructionSet& test);

    // prints state of moveBuilder
    void printMoveBuilderState();

    using InstructionSetQueue = std::queue<std::unique_ptr<MoveInstructionSet>>;

    InstructionSetQueue testInstructionSetQueue;
    MoveBuilder moveBuilder;
};

// ===============================================================================================


// function to use to test the move builder
inline void performMoveBuilderTest()
{
    auto tester = MoveBuilderTester{};
    auto startFen = std::string {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    auto testFen  = std::string {"rnb1kbnr/p1p1pppp/1p1p4/2P1q3/3P4/8/PP2PPPP/RNBQKBNR w KQkq - 0 1"};

    // move e2e4 from start position: pull(e2) put(e4)
    tester.addInstructionSet (startFen, "simple_move_non_cap", "e2e4", {"-e2", "+e4"});

    // move e2e4 from start position, with doubt: pull(a2) put(a2) pull(e2) put(e4)
    tester.addInstructionSet (startFen, "doubt_move_non_cap", "e2e4", {"-a2", "+a2", "-e2", "+e4"});

    // move d4e5 from start position, without doubt, with capture
    // pull(d4) pull(e5) put(e5)
    tester.addInstructionSet (testFen, "certain_move_cap", "d4e5", {"-d4", "-e5", "+e5"});

    // same move as above, with a doubt:
    // pull(c5) pull(d6) put(d6) put(c5) -> pull(d4) pull(e5) put(e5)
    tester.addInstructionSet (testFen, "doubt_move_cap", "d4e5", {"-c5", "-d6", "+d6", "+c5", "-d4", "-e5", "+e5"});

    // same move as about, with a doubt and recovery mode
    // pull(c5) pull(d6) pull(b1) [enter_rec] put(b1) [exit_rec] put(d6) put(c5) -> pull(d4) pull(e5) put(e5)
    tester.addInstructionSet (testFen, "doubt_move_cap_recovery", "d4e5", {"-c5", "-d6", "-b1", "+b1", "+d6", "+c5", "-d4", "-e5", "+e5"});

    // test invalid input
    tester.addInstructionSet (testFen, "incomplete_move", "invalid", {"-c5", "-d6", "-b1", "+d6", "+c5", "-d4", "-e5", "+e5"});

    // start the test engine
    tester.performAllTests();
}

#endif //CHESS_ENGINE_MOVEBUILDERTESTER_H
