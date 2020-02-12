//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CHECK_SEMANTICS_HPP
#define NYARUGA_UTIL_CHECK_SEMANTICS_HPP

#pragma once

// ランダムな値を入れた配列と T -> bool な関数を使って、セマンティクスが
// 満たされているかをテストする
// 例えば、ある型の集合が群の構造を持つか確かめたいとき、Concept で制限できるのは演算(+)が定義
// されているかどうかといった程度で、結合則が満たされているかを確かめることができない
// そのような場合に結合則をテストするような関数を使ってテストするためのヘルパ関数がここに定義されている

#include <utility>
#include <concepts>
#include <array>
#include <boost/hana/functional/overload.hpp>

namespace nyaruga::util {

namespace detail {
 
template <typename ArrayLike, typename Fn, std::size_t... I>
constexpr bool check_semantics_impl(ArrayLike&& a, Fn&& f, std::index_sequence<I...>)
{
    return ( f(a[I]) && ... );
}

} // namespace detail


namespace check_semantics_ { // protection from unintended ADL

static inline constexpr auto check_semantics = boost::hana::overload(
[]<std::size_t N>(auto&& a, auto&& f) requires requires() { { f(a[0]) } -> std::same_as<bool>; }
{
    return detail::check_semantics_impl(std::forward<decltype(a)>(a), std::forward<decltype(f)>(f), std::make_index_sequence<N>{});
},

[]<std::size_t N, typename T, template <class, std::size_t> class Array>
(const Array<T, N>& a, auto&& f) requires requires() { { f(a[0]) } -> std::same_as<bool>; }
{
    return detail::check_semantics_impl(std::forward<decltype(a)>(a), std::forward<decltype(f)>(f), std::make_index_sequence<N>{});
},
    
[]<std::size_t N, typename T, template <class, std::size_t> class Array>
(T (&a)[N], auto&& f) requires requires() { { f(a[0]) } -> std::same_as<bool>; }
{
    return detail::check_semantics_impl(std::forward<decltype(a)>(a), std::forward<decltype(f)>(f), std::make_index_sequence<N>{});
},
    
[]<std::size_t N, typename T, template <class, std::size_t> class Array>
(T (&a)[N], auto&& f) requires requires() { { f(a[0]) } -> std::same_as<bool>; }
{
    return detail::check_semantics_impl(std::forward<decltype(a)>(a), std::forward<decltype(f)>(f), std::make_index_sequence<N>{});
}

);

} // namespace check_semantics_

using namespace check_semantics_;

/* 使用方法

// 値をランダムに生成する（これは、コンパイル時乱数の配列を生成する <random_array> を使うとよい）
constexpr std::array<int, 5> a = {2, 4, 5, 2, 5};

// 何かの semantics を調査して、bool を返す関数を用意する（一引数関数とする）
auto f = [](int a){ return a == a; };

// 配列のそれぞれの値に対して、semantics を満たすかどうかを確かめる
static_assert(check_semantics(a, f), "");

*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_CHECK_SEMANTICS_HPP
