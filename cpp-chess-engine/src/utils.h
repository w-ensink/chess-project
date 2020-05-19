//
// Created by Wouter Ensink on 13/05/2020.
//

#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include <iostream>


template <typename... T>
auto print (T&&... args)
{
    auto helper = [] (auto&& arg) { std::cout << arg << " "; };
    (helper (args), ...);
    std::cout << '\n';
}


#endif //CHESS_ENGINE_UTILS_H
