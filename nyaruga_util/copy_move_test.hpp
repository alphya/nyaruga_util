
/*!
 * @file 
 * @brief 
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_COPY_MOVE_TEST_HPP
#define NYARUGA_UTIL_COPY_MOVE_TEST_HPP

#pragma once

#include <iostream>

namespace nyaruga {

namespace util {

namespace { // for static

int total_copy = 0;
int total_move = 0;
int total_construct = 0;

} // unnamed namespace

namespace copy_move_test_ {

struct copy_move_test {
   template <typename T>
   copy_move_test(const T &) { ++total_construct; }
   copy_move_test() { ++total_construct; }
   copy_move_test(const copy_move_test &) { ++total_copy; }
   copy_move_test(copy_move_test &&) { ++total_move; }
   auto & operator=(const copy_move_test &) noexcept { return *this; }
   auto & operator=(copy_move_test &&) noexcept { return *this; }
   bool operator==(const copy_move_test &) const = default;
   std::string hello() { return " "; }
};

} // namespace copy_move_test_

inline void print_copy_move_and_reset()
{
   std::cout << "total_construct: " << total_construct
             << " total_copyed: " << total_copy
             << " total_moved: " << total_move << std::endl;
   total_construct = 0;
   total_copy = 0;
   total_move = 0;
}

using namespace copy_move_test_;

} // nyaruga :: util

#endif // NYARUGA_UTIL_COPY_MOVE_TEST_HPP
