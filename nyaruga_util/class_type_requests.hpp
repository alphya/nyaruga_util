
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP
#define NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP

#pragma once

namespace nyaruga {

namespace util {

template <class Trait, bool Cond = true>
struct this_class {
   static_assert(Trait::value == Cond, "The class type is not requested class.");
};

} // namespace util

} // namespace nyaruga

/*  クラスがある要件(type_traits)を満たすことを、static_assert によって保証します。

#include "../../nyaruga::util/class_type_requests.hpp"
#include <type_traits>

using namespace nyaruga::util;
using namespace std;

class my_class
   : this_class<is_class<my_class>> { // well-formd
};

class my_class
   : this_class<negation<is_class<my_class>>> { // ill-formd
};

*/

/*
template <typename T> 
struct satisfy_Concept {
   constexpr satisfy_Concept(){ static_assert( "Some Concept here" ); }
};

struct mohu : satisfy_Concept<mohu> {};
*/

#endif // #ifndef NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP
