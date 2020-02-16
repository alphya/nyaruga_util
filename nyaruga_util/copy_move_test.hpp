//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_COPY_MOVE_TEST_HPP
#define NYARUGA_UTIL_COPY_MOVE_TEST_HPP

#pragma once

#include <iostream>

namespace nyaruga::util {

namespace { // for static

int total_copy = 0;
int total_move = 0;
int total_construct = 0;

struct copy_move_test
{
   template <typename T>
   copy_move_test(const T&) { ++total_construct; }
   copy_move_test() { ++total_construct; std::cout << "default "; }
   copy_move_test(const copy_move_test&) { ++total_copy; std::cout << "copy "; }
   copy_move_test(copy_move_test&&) { ++total_move; std::cout << "move "; }
};

struct print { 
   ~print() { 
      std::cout << "total_construct: " << total_construct 
                << " total_copyed: " << total_copy 
                << " total_moved: " << total_move; }
} printer; // プログラム終了時に標準出力に出力する

} // unnamed namespace

} // nyaruga :: util

#endif // NYARUGA_UTIL_COPY_MOVE_TEST_HPP
