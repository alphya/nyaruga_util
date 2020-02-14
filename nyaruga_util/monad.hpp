//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MONAD_HPP
#define NYARUGA_UTIL_MONAD_HPP

#pragma once

#include <concepts>
#include <nyaruga_util/category.hpp>

// ----------- 注意 -----------
// ここのページは作成途中です
// ----------------------------

namespace nyaruga::util {

namespace monad_ {

// すごく単純な例です。コピペして新しいモナドを作ってください

// 組(T, η, μ) をモナドとすると、

template <template <class> class T, typename X>
struct monad {
   static inline category::make_eta<T, X> eta; // η. Haskell のモナドの型クラスにおける return

   static inline category::make_mu_T<T, T<X> (*)(X)> mu_T; // Haskell における >>=, operator >> の実装

   template <category::kleisli_morphism<T> Mor>
   constexpr decltype(auto) friend operator>>(const T<X> & m, const Mor & g) { return monad::mu_T(m, g); };
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

} // namespace monad_

using namespace monad_;

} // nyaruga :: util

#endif // NYARUGA_UTIL_MONAD_HPP
