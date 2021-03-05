
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_IOTA_PP_HPP
#define NYARUGA_UTIL_IOTA_PP_HPP

#pragma once

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#define PRED(r, state) \
   BOOST_PP_NOT_EQUAL( \
      BOOST_PP_TUPLE_ELEM(2, 0, state), \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 1, state)) \
   ) \

#define OP(r, state) \
   ( \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 0, state)), \
      BOOST_PP_TUPLE_ELEM(2, 1, state) \
   ) \

// expands to FIRST, FIRST+1, FIRST+2, ..., END-1
#define MACRO(r, state) (BOOST_PP_TUPLE_ELEM(2, 0, state))
#define IOTA(FIRST, END) BOOST_PP_SEQ_ENUM(BOOST_PP_FOR((FIRST, BOOST_PP_DEC(END)), PRED, OP, MACRO))

/* how to use

#include <boost/preprocessor/stringize.hpp>
#include <vector>
#include <iostream>

std::vector<int> a = {IOTA(0,10)};
// expands to 
// std::vector<int> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

int main(){
  std::cout << BOOST_PP_STRINGIZE(IOTA(0,10)); // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

*/

#endif // #ifndef NYARUGA_UTIL_IOTA_PP_HPP