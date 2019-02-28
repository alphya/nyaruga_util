
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP
#define NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP

#pragma once

namespace nyaruga_util {

template <class Trait, bool Cond = true>
struct this_class {
   static_assert(Trait::value == Cond, "The class type is not requested class.");
};

} // namespace nyaruga_util

/*  クラスがある要件(type_traits)を満たすことを、static_assert によって保証します。

#include "../../nyaruga_util/class_type_requests.hpp"
#include <type_traits>

using namespace nyaruga_util;
using namespace std;

class my_class
   : this_class<is_class<my_class>> { // well-formd
};

class my_class
   : this_class<negation<is_class<my_class>>> { // ill-formd
};

*/

#endif // #ifndef NYARUGA_UTIL_CLASS_TYPE_REQUESTS_HPP
