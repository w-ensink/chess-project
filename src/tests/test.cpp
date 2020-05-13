//
// Created by Wouter Ensink on 13/05/2020.
//


#include "../shallow_blue_interface.hpp"
#include "../utils.h"
#include <vector>
#include <map>

struct TestPosition {

};

auto testMoveValidation()
{
    std::map<std::string, bool> moveOptions {
        {"e2e4", true},
        {"e2e5", false},
        {"b1c3", true},
        {"e7e5", false}
    };

    for (auto& move : moveOptions)
        if (eon::isValidMove(move.first) != move.second) {
            print("Move Validation Test Failed");
            return false;
        }

    return true;
}

int main()
{
    eon::initChessEngine();

    if (testMoveValidation()) {
        print("Move Validation Test Succeeded");
    }

    return 0;
}