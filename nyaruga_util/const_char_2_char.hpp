
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_CONST_CHAR_2_CHAR_HPP
#define NYARUGA_UTIL_CONST_CHAR_2_CHAR_HPP

#pragma once

namespace nyaruga {

namespace util {

/* for copy and paste

#include <vector>
#include <cstdio> // snprintf

std::string to = "some text";
std::vector<char> str(to.size());
std::snprintf(str.data(), str.size(), "%s", to.c_str());
char * to = str.data();

or

const char from[] = "some text";
std::vector<char> str(sizeof from);
std::snprintf(str.data(), str.size(), "%s", from);
char * to = str.data();

*/

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_CONST_CHAR_2_CHAR_HPP
