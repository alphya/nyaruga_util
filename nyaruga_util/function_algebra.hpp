//              Copyright (c) 2021 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_FUNCTION_ALGEBRA_HPP
#define NYARUGA_UTIL_FUNCTION_ALGEBRA_HPP

#include <functional>
#include <iostream>
#include <concepts>
#include <ranges>
#include <nyaruga_util/algebra.hpp>

namespace nyaruga {

namespace util {

namespace detail {


template <typename Signature>
struct function;

template <typename Cod, typename ... Dom>
struct function<Cod(Dom...)> : std::function<Cod(Dom...)>
{
    using std::function<Cod(Dom...)>::function;
};

// f_*
template <typename Cod1, typename ... Dom1, typename Cod2, typename ... Dom2>
function<Cod2(Dom1...)> operator > (const function<Cod1(Dom1...)>& f1, const function<Cod2(Dom2...)>& f2) noexcept requires requires(Dom1 ... d) { f2(f1(d...));  }
{ return function<Cod2(Dom1...)>([=](Dom1 ... doms)mutable{ return f2(f1(doms...)); }); }

// f^*
template <typename Cod1, typename ... Dom1, typename Cod2, typename ... Dom2>
function<Cod1(Dom2...)> operator < (const function<Cod1(Dom1...)>& f1, const function<Cod2(Dom2...)>& f2) noexcept requires requires(Dom2 ... d) { f1(f2(d...));  }
{ return function<Cod1(Dom2...)>([=](Dom2 ... doms)mutable{ return f1(f2(doms...)); }); }

//  関数環としての積
template <typename Cod, typename ... Dom>
function<Cod(Dom...)> operator * (const function<Cod(Dom...)>& f1, const function<Cod(Dom...)>& f2) noexcept requires requires(Dom ... d) { (f1(d...))*(f2(d...));  }
{ return function<Cod(Dom...)>([=](Dom ... d)mutable{ return f1(d...)*(f2(d...)); }); }

//  関数環としての和
template <typename Cod, typename ... Dom>
function<Cod(Dom...)> operator + (const function<Cod(Dom...)>& f1, const function<Cod(Dom...)>& f2) noexcept requires requires(Dom ... d) { (f1(d...))+(f2(d...));  }
{ return function<Cod(Dom...)>([=](Dom ... d)mutable{ return f1(d...)+(f2(d...)); }); }

//  関数環としての差
template <typename Cod, typename ... Dom>
function<Cod(Dom...)> operator - (const function<Cod(Dom...)>& f1, const function<Cod(Dom...)>& f2) noexcept requires requires(Dom ... d) { (f1(d...))-(f2(d...));  }
{ return function<Cod(Dom...)>([=](Dom ... d)mutable{ return f1(d...)-(f2(d...)); }); }

//  関数環を他元環と見たときのスカラー倍
template <commutative_ring R, typename Cod, typename ... Dom>
function<Cod(Dom...)> operator * (const R& a, const function<Cod(Dom...)>& f) noexcept requires requires(Dom ... d) { a*(f(d...));  }
{ return function<Cod(Dom...)>([=](Dom ... d)mutable{ return a*(f(d...)); }); }

// 上記の定義から構成する関数環
template  <commutative_ring R, typename Cod, typename ... Dom>
struct function_algebra : function<Cod(Dom...)>
{
    using function<Cod(Dom...)>::function;
    operator function<Cod(Dom...)>() const noexcept { *this; }
};

template  <commutative_ring R = double, typename Cod, typename ... Dom>
function_algebra(Cod(*)(Dom...)) -> function_algebra<R, Cod, Dom...>;

// algebra の要件を満たすこと
static_assert(algebra<double,decltype(function_algebra(+[](int a){ return a*a;}))>);

} // namespace detail

using detail::function;
using detail::function_algebra;

} // namespace util

} // namespace nyaruga

/* how to use

int main()
{
    auto  f1 = function_algebra(+[](int a){ return a*a;}); // テンプレート引数推論を使うときは、ラムダ式は一旦関数ポインタに変換する
    auto  f2 = function_algebra{+[](int a){ return a + 1;}};
    
    std::cout << (3 * f2)(3) << "\n";
    std::cout << (f1 + f2)(3) << "\n";

}

*/

#endif // #ifndef NYARUGA_UTIL_FUNCTION_ALGEBRA_HPP