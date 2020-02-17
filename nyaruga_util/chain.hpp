//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CHAIN_HPP
#define NYARUGA_UTIL_CHAIN_HPP

#pragma once

#include <compare>
#include <nyaruga_util/category.hpp>

// メソッドチェインができます

namespace nyaruga::util {

namespace chain_ {

template <class T>
class chain {
private: 
   T m_val;
public:
   constexpr chain(const T & x) noexcept : m_val(x) {}
   constexpr chain(T && x) noexcept : m_val(std::forward<T>(x)) {}
   constexpr auto& unwrap() noexcept { return m_val; }
   constexpr auto operator <=> (const chain&) const noexcept = default;
};

template <class T, class F>
   requires category::kleisli_morphism_from<F, chain, T>
constexpr auto operator>=(const chain<T> & x, F && f) noexcept(noexcept(f(x.unwrap())))
{
   return f(x.unwrap());
}

template <class T, class F>
   requires category::kleisli_morphism_from<F, chain, T>
constexpr auto operator>=(chain<T> && x, F && f) noexcept(noexcept(f(std::move(x.unwrap()))))
{
   return f(std::move(x.unwrap()));
}


template <class T>
constexpr auto ret(T&& x) noexcept -> decltype(chain(std::forward<T>(x)))
{
   return chain(std::forward<T>(x));
}

template <class T, class F>
   requires category::morphism_from<F, T> && category::kleisli_object<category::apply_morphism<T, F>, chain>
constexpr auto operator|(const chain<T> & x, F && f) noexcept(noexcept(f(x.unwrap()))) -> chain<category::apply_morphism<T, F>>
{
   return f(x.unwrap());
}

template <class T, class F>
   requires category::morphism_from<F, T> && category::kleisli_object<category::apply_morphism<T, F>, chain>
constexpr auto operator|(chain<T> && x, F && f) noexcept(noexcept(f(std::move(x.unwrap())))) -> chain<category::apply_morphism<T, F>>
{
   return f(std::move(x.unwrap()));
}

// nyaruga_util/category におけるモナドの要件を満たしているか(ret と >= が期待する動作をするか)の確認
// MSCV ではなぜか失敗します...
// static_assert(category::monad<chain, int, double>);
// static_assert(category::monad<chain, float, std::string>);

}

using namespace chain_;

/*

// chain がモナド則を満たすかどうかのテスト
template <template <class> class M, typename T, typename U>
requires requires(T x, U y) { {x + y} -> std::same_as<U>; } // この例では、この演算を使っているため
constexpr bool monad_rule(T x, U) // monad がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY は任意
   auto f = [](const T& x) -> M<T> { return {x}; };
   auto g = [](const T& x) -> M<U> { return {x * 3.14}; };

   bool hidari_id = (ret(x) >= f) == f(x); // 左単位元律
   bool migi_id = (ret(x) >= ret<T>) == ret(x); // 右単位元律
   auto a = ( ret(x) >= f ) >= g;
   auto b = ret(x) >= ( [f, g](T x){
      return (f(x) >= g); } );
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

int main()
{
   chain<int> c{6};
   std::cout << (c >= [](int a){ return chain{a+1}; }).val;
   std::cout << (c | [](int a){ return a+1; }).val;

   std::cout << std::boolalpha << monad_rule<chain>(14, 3.15);
}

// テストその2

// chain がモナド則を満たすかどうかのテスト
template <template <class> class M, typename T, typename U>
constexpr bool monad_rule(T x, U) // monad がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY は任意
   auto f = [](const T&) -> M<T> { std::cout << "callf:constructed "; return { 0 }; };
   auto g = [](const T&) -> M<U> { std::cout << "ccallg:onstructed "; return { 3.14 }; };

   bool hidari_id = (ret(x) >= f) == f(x); // 左単位元律
   bool migi_id = (ret(x) >= ret<T>) == ret(x); // 右単位元律
   auto a = (ret(x) >= f) >= g;
   auto b = ret(x) >= ([f, g](T x) {
      return (f(x) >= g); });
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

static int total_copy = 0;
static int total_move = 0;
static int total_construct = 0;

struct copy_move_test
{
   template <typename T>
   copy_move_test(const T&) {  }
   copy_move_test() { ++total_construct; std::cout << "default "; }
   copy_move_test(const copy_move_test&) { ++total_copy; std::cout << "copy "; }
   copy_move_test(copy_move_test&&) { ++total_move; std::cout << "move "; }
   bool operator == (const copy_move_test&) const = default;
};

int main()
{
   chain<copy_move_test> {6} >= [](const copy_move_test&) { return chain{ copy_move_test(1) }; };
   chain<copy_move_test> {6} | [](const copy_move_test&) { return copy_move_test(1); };

   std::cout << std::boolalpha << monad_rule<chain>(copy_move_test(14), copy_move_test(3.15));

   std::cout << "total_construct: " << total_construct << " total_copyed: " << total_copy << " total_moved: " << total_move;
}


*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
