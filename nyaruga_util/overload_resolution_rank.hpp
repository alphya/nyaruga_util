
//              Copyright (c) 2019 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_OVERLOAD_RESOLITION_RANK_HPP
#define NYARUGA_UTIL_OVERLOAD_RESOLITION_RANK_HPP

#pragma once


// please look
// https://yohhoy.hatenadiary.jp/entry/20190420/1555755625

namespace nyaruga {
namespace util {
namespace overload_resolution_rank_ { // protection from unintended ADL

// You can control the priority of overload resolution.
template <int Rank>
struct overload_resolution_rank : overload_resolution_rank<Rank - 1> {
};
template <>
struct overload_resolution_rank<0> {
};

using overload_resolution_rank = overload_resolution_rank_::overload_resolution_rank;

} // namespace overload_resolution_rank_
} // namespace util
} // namespace nyaruga

/*   usage
#include <iostream>
#include <string>
#include <type_traits>


namespace my_namespace
{

template<typename T>
struct my_struct
{
    constexpr my_struct() noexcept = default;
    constexpr my_struct(T && v) noexcept : value(std::forward<T>(v)) {}
    T value;
};

namespace detail
{

template<typename T, typename U>
constexpr auto compare_impl(nyaruga::util::overload_resolution_rank<2>, 
                            const my_struct<T> & lhs, const my_struct<U> & rhs) noexcept 
    -> decltype(lhs.value < rhs.value)
{
    return lhs.value < rhs.value;
}

template<typename T, typename U>
constexpr auto compare_impl(nyaruga::util::overload_resolution_rank<1>, 
                            const my_struct<T> & lhs, const my_struct<U> & rhs) noexcept 
    -> void
{
    static_assert([]{return false;}(), "Can not compareable my_struct<T>::value and my_struct<U>::value");
}

} // namespace detail

template<typename T, typename U>
constexpr auto operator<(const my_struct<T> & lhs, const my_struct<U> & rhs) noexcept 
    -> decltype(detail::compare_impl(nyaruga::util::overload_resolution_rank<2>{}, lhs, rhs))
{
    return detail::compare_impl(nyaruga::util::overload_resolution_rank<2>{}, lhs, rhs);
}


}  // namespace my_namespace


auto main() -> int
{
    constexpr my_namespace::my_struct<double> m(30);
    constexpr my_namespace::my_struct<int> n(11);
    // constexpr my_namespace::my_struct<int*> n(nullptr); // static assertion failed in operator <

    constexpr auto r = n < m;
    static_assert(r == true);
}

*/

#endif // #ifndef NYARUGA_UTIL_OVERLOAD_RESOLITION_RANK_HPP
