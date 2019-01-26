
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_HAS_ITERATOR_HPP
#define NYARUGA_UTIL_HAS_ITERATOR_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
#endif

#include <type_traits>

namespace nyaruga_util {

template <class, class = void>
struct has_iterator : std::false_type {
};

template <class T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {
};

} // namespace nyaruga_util

/*

#include <vector>
int main()
{
        static_assert(
                has_iterator<std::vector<int>>::value,
                "vectorはiterator型を持っている");

        static_assert(
                !has_iterator<int>::value,
                "intはiterator型を持っていない");
}

*/

#endif // #ifndef NYARUGA_UTIL_HAS_ITERATOR_HPP