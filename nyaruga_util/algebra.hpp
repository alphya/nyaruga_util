//              Copyright (c) 2021 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_ALGEBRA_HPP
#define NYARUGA_UTIL_ALGEBRA_HPP

#include <functional>
#include <iostream>
#include <concepts>
#include <ranges>
#include <ratio>
#include <complex>
#include <tcb/rational.hpp> // https://github.com/tcbrindle/rational.git
#include <type_traits>
#include <array>
#include <algorithm>
#include <utility>
#include <boost/hana/functional/arg.hpp>

namespace nyaruga {

namespace util {

namespace detail {

template <std::unsigned_integral  T = unsigned int>
struct N {
    using value_type = T;
    T value; 
    friend constexpr auto operator<=>(const N&, const N&) noexcept = default;
    constexpr operator T() const noexcept { return value; }
    constexpr N(T&& v) noexcept : value(std::forward<decltype(v)>(v)) {}
};

template <std::floating_point T = double>
struct R {
    using value_type = T;
    T value; 
    friend constexpr auto operator<=>(const R&, const R&) noexcept = default;
    constexpr operator T() const noexcept { return value; }
    constexpr R(T&& v) noexcept : value(std::forward<decltype(v)>(v)) {}
};

template <std::integral T = int>
struct Z {
    using value_type = T;
    T value; 
    friend constexpr auto operator<=>(const Z&, const Z&) noexcept = default;
    constexpr operator T() const noexcept { return value; }
    constexpr Z(T&& v) noexcept : value(std::forward<decltype(v)>(v)) {}
};

template <typename T>
concept  arithmetic = std::is_arithmetic_v<T>;

} // namespace detail

// Number

using detail::N;

using detail::Z;

using detail::R;

template <std::integral T = int>
using Q = tcb::rational<T>;

template <detail::arithmetic T = double>
using C = std::complex<T>;


namespace detail {

// 足し算ができる
template <class T, class U>
concept weak_addable = requires (const std::remove_reference_t<T>& a, const std::remove_reference_t<U>& b) {
  {a + b} -> std::common_with<T>;
  {b + a} -> std::common_with<T>;
  {a + b} -> std::common_with<U>;
  {b + a} -> std::common_with<U>;
};

// 引き算ができる
template <class T, class U>
concept weak_subtractable = requires (const std::remove_reference_t<T>& a, const std::remove_reference_t<U>& b) {
  {a - b} -> std::common_with<T>;
  {b - a} -> std::common_with<T>;
  {a - b} -> std::common_with<U>;
  {b - a} -> std::common_with<U>;
};

// 掛け算ができる
template <class T, class U>
concept weak_multipliable = requires (const std::remove_reference_t<T>& a, const std::remove_reference_t<U>& b) {
  {a * b} -> std::common_with<T>;
  {b * a} -> std::common_with<T>;
  {a * b} -> std::common_with<U>;
  {b * a} -> std::common_with<U>;
};

//割り算ができる
template <class T, class U>
concept weak_dividable = requires (const std::remove_reference_t<T>& a, const std::remove_reference_t<U>& b){
  {a / b} -> std::common_with<T>;
  {b / a} -> std::common_with<T>;
  {a / b} -> std::common_with<U>;
  {b / a} -> std::common_with<U>;
};

} // namespace detail


// concept

template <typename T>
concept addable = detail::weak_addable<T,T>;

template <typename T>
concept subtractable = detail::weak_subtractable<T,T>;

template <typename T>
concept multipliable = detail::weak_multipliable<T,T>;

template <typename T>
concept dividable = detail::weak_dividable<T,T>;

template <class T, class U>
concept addable_with =
  addable<T> &&
  addable<U> &&
  std::common_reference_with<
    const std::remove_reference_t<T>&,
    const std::remove_reference_t<U>&> &&
  addable<
    std::common_reference_t<
      const std::remove_reference_t<T>&,
      const std::remove_reference_t<U>&>> &&
  detail::weak_addable<T, U>;

template <class T, class U>
concept subtractable_with =
  subtractable<T> &&
  subtractable<U> &&
  std::common_reference_with<
    const std::remove_reference_t<T>&,
    const std::remove_reference_t<U>&> &&
  subtractable<
    std::common_reference_t<
      const std::remove_reference_t<T>&,
      const std::remove_reference_t<U>&>> &&
  detail::weak_subtractable<T, U>;

template <class T, class U>
concept multipliable_with =
  multipliable<T> &&
  multipliable<U> &&
  std::common_reference_with<
    const std::remove_reference_t<T>&,
    const std::remove_reference_t<U>&> &&
  multipliable<
    std::common_reference_t<
      const std::remove_reference_t<T>&,
      const std::remove_reference_t<U>&>> &&
  detail::weak_multipliable<T, U>;

template <class T, class U>
concept dividable_with =
  dividable<T> &&
  dividable<U> &&
  std::common_reference_with<
    const std::remove_reference_t<T>&,
    const std::remove_reference_t<U>&> &&
  dividable<
    std::common_reference_t<
      const std::remove_reference_t<T>&,
      const std::remove_reference_t<U>&>> &&
  detail::weak_dividable<T, U>;

template <typename T>
concept group = std::equality_comparable<T> &&
                                 addable<T> &&
                                 subtractable<T>;

template <typename T>
concept abelian_group =  // C++ 上で普通の群と見分ける方法なし
                                 std::equality_comparable<T> &&
                                 addable<T> &&
                                 subtractable<T>;

template <typename T>
concept ring = std::equality_comparable<T> &&
                             addable<T> &&
                             subtractable<T> && 
                             multipliable<T>;

template <typename T>
concept commutative_ring =  // C++ 上で普通の環と見分ける方法なし
                             std::equality_comparable<T> &&
                             addable<T> &&
                             subtractable<T> && 
                             multipliable<T>;

template <typename T>
concept field = std::equality_comparable<T> &&
                              addable<T> && 
                              subtractable<T> && 
                              multipliable<T> && 
                              dividable<T>;


namespace detail {

template <ring A = R<double>, size_t rank = 1>
struct module : std::array<A, rank>
{
    using std::array<A,rank>::array;
    constexpr module() noexcept = default;
    constexpr module(const std::array<A,rank>& a) : std::array<A,rank>::array(a) {};
    constexpr module(std::array<A,rank>&& a) : std::array<A,rank>::array(std::move(a)) {};
    template <std::same_as<A> ... T>
    constexpr module(T ... list) : std::array<A,rank>::array({list...}) {};
    friend constexpr auto operator<=>(const module&, const module&) noexcept = default;

};

template <typename ... T>
module(T ... list) -> module<std::remove_cvref_t<decltype(boost::hana::arg<1>(list...))> ,sizeof...(list)>;

namespace my_detail {

template <typename T, size_t N, size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>
    array_plus_impl(const std::array<T,N>& a, const std::array<T,N>& b, std::index_sequence<I...>)
{
    return { {(a[I]+b[I])...} };
}

template <typename T, size_t N, size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>
    array_mult_impl(const T& a, const std::array<T,N>& b, std::index_sequence<I...>)
{
    return { {(a*b[I])...} };
}

}
 
template <typename T, size_t N>
constexpr module<T,N>  operator+(std::array<T,N>& a, std::array<T,N>& b)
{
    return my_detail::array_plus_impl(a, b,std::make_index_sequence<N>{});
}

template <typename T, size_t N>
constexpr module<T,N>  operator*(const T& a, std::array<T,N>& b)
{
    return my_detail::array_mult_impl(a, b,std::make_index_sequence<N>{});
}

} // namespace detail


// 加群
template <ring A, size_t rank>
using module = detail::module<A, rank>;

// 多元環または代数(algebra) 係数環の可換性を確かめられないので、C++上だと加群と変わらず
template <commutative_ring A, size_t rank>
using algebra = detail::module<A, rank>;


} // namespace util

} // namespace nyaruga

/* how to use 

int main(){

    N q1 = 3u;
    N q2 = 3u;
    N q3 = 4u;
    N q4 = 6u;

    module m1 = {q1,q2};
    module m2 = {q3,q4};

   std::cout <<(m1+m2)[1] << "\n";
   std::cout << (q1*m2)[0] << "\n";
};

*/

#endif // #ifndef NYARUGA_UTIL_ALGEBRA_HPP
