//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MONAD_HPP
#define NYARUGA_UTIL_MONAD_HPP

#pragma once

#include <concepts>
#include <unwrap_helper_idx.hpp>

namespace nyaruga::util {

// 組(T, η, μ) をモナドとすると、

// T は対象が型、射が型 X から 型 Y への関数となるような圏を、
// 対象が monad<型>、射が monad<X> 型から monad<Y> 型への関数になるような圏へと移す関手

template <typename X>
requires requires(X a, X b) { a == b; }
struct monad // monad は型 X から 型 monad<X> への関手。モナド(T, η, μ) における T
{
   const X x;
    
   static inline constexpr auto eta = [](const auto& x) -> monad<std::remove_cvref_t<decltype(x)>> 
   { return {x}; }; // η. Haskell のモナドの型クラスにおける return
    
   static inline constexpr auto mu = []<class U>(const monad<U>& m) -> U { return m.x; }; // μ
    
   // g : X -> Y と引数の TX から TY を計算して返す。関手の g -> Tg の役割をする
   // (Tg)(m) に対応。Tg : TX -> TY は (>> g) に対応
   template <typename Mor>
   requires requires(Mor g, monad m) { { g(mu(m)) }; } 
   constexpr decltype(auto) friend operator >> (const monad& m, const Mor& g) { return eta(g(mu(m))); }

   // Haskell のモナドの型クラスにおける >>=
   // | の引数は monad<X> とクライスリ圏における射 Mor: X -> TY で、これらから monad<Y> を出力する
   // | は (μ◦T(-))(-) : Mor × monad<X> -> monad<Y> に対応すると思われる
   // g : X -> TY または X -> monad<Y>
   // m : TX または monad<X>
   // Tg : TX -> TTY または monad<X> -> monad<monad<Y>>, (>> g) でこれを作成している
   // μ◦Tg : TX -> TY または monad<X> -> monad<Y> ; (| g) に対応。mu と (>> g) でこれを作成している
   // (μ◦Tg)(m) : TY または monad<Y> ; (m | g)、または (μ◦Tg)(m) に対応。これは TY, または monad<Y> である
   // 蛇足：クライスリ圏における射は、f : X -> TY という形で、射 g : Y -> TZ と合成すると
   // μ◦Tg◦f : X -> TZ という射になる。μ◦T(次の射)◦(前の射) というように、前の射に μ◦T(次の射) を
   // 合成することで行う。| は、クライスリ圏における対象 m を、その出発点が μ(m) な射 g を用いて送る関数だと考えられる。
   // (| g) : TX -> TY そのものはクライスリ圏における射にならないことに注意
   template <typename Mor>
   requires requires(Mor g, X x) { { g(x) } -> std::same_as<monad<unwrap_helper_idx<0, decltype(g(x))>>>; } 
   constexpr decltype(auto) friend operator | (const monad& m, const Mor& g) { return mu(m >> g); };

   template<typename Y> requires requires(X x, Y y) { x == y; }
   constexpr bool friend operator == (const monad& lhs, const monad<Y>& rhs) { 
      return std::is_same_v<Y, X> && (mu(lhs) == mu(rhs)); 
   };
};

/*

// 適宜テスト用に書き換えてください
template <template <class> class M, typename T, typename U>
requires requires(T x, U y) { {x + y} -> std::same_as<U>; } // この例では、この演算を使っているため
constexpr bool monad_rule(const T& x, const U& y) // monad がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY はクライスリ圏における射
   auto f = [](const T& x) -> M<T> { return {x}; };
   auto g = [&y](const T& x) -> M<U> { return {x + y}; };

   bool hidari_id = (M<T>::eta(x) | f) == f(x); // 左単位元律
   bool migi_id = (M<T>::eta(x) | M<T>::eta) == M<T>::eta(x); // 右単位元律
   auto a = ( M<T>::eta(x) | f ) | g;
   auto b = M<T>::eta(x) | ( [f, g](T x){ return (f(x) | g); } );
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

static_assert(monad_rule<monad>(10, 3.14), "");
static_assert(monad_rule<monad>(float(14.33), 3.3), "");
bool tmp1 = monad_rule<monad>(std::string("hello"), std::string("world"));

*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_MONAD_HPP
