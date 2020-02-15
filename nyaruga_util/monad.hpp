//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MONAD_HPP
#define NYARUGA_UTIL_MONAD_HPP

#pragma once

#include <concepts>
#include <nyaruga_util/category.hpp>

namespace nyaruga::util {

namespace monad_ {

// すごく単純な例です。コピペして新しいモナドを作ってください

// 組(T, η, μ) をモナドとすると、

// T は対象が型、射が型 X から 型 Y への関数となるような圏を、
// 対象が monad<型>、射が monad<X> 型から monad<Y> 型への関数になるような圏へと移す関手

// CRTP でこれを継承して、エラーが出ないもので、下記のモナド測を満たすものがモナド
// nyaruga_util/category では、(static) メンバ関数 ret : T -> monad<T>, fmap : (X -> Y) -> (monad<X> -> mondo<Y>),
// operator >> : 引数が monad<X>, f : X -> monad<Y> で戻り値が monad<Y> の 3 種類のメンバを使ってモナドとしている
template <template <class> class T, typename X, typename Y>
struct monad 
{
   monad() { static_assert(category::monad<T, X, Y>, "The class is not a monad."); }
};

// モナド測
/*
// f, g : X -> TY はクライスリ圏における射とすうとき、
auto f = [](const T& x) -> M<T> { return {{},x+10}; };
auto g = [](const T& x) -> M<U> { return {{},x*3.14}; };

(M<T>::ret(x) >> f) == f(x); // 左単位元律
M<T>::ret(x) >> M<T>::ret) == M<T>::ret(x); // 右単位元律
(( M<T>::ret(x) >> f ) >> g) == (M<T>::ret(x) >> ( [f, g](T x){ return (f(x) >> g); } )); // 結合律
*/

// モナドの例。メソッドチェインができる
template <typename X>
struct chain : monad<chain, X, X> {
   const X x;

   static auto ret(X x) { return chain<X>{ {}, x }; }; // η. Haskell のモナドの型クラスにおける return

   template <category::morphism_from<X> Mor>
   auto fmap(const Mor & f)
   {
      return [f, this](chain<X> c) { return chain<category::apply_morphism<X, Mor>>{ {}, f(c.x) }; };
   }

   constexpr bool operator==(const chain & other) const { return x == other.x; };

   // >> は Haskell における >>=
   template <category::morphism_from<X> KleisliMor>
   requires category::functor<chain, X, category::apply_mu<chain, category::apply_kleisli_morph<chain, X, KleisliMor>>> constexpr auto friend operator>>(const chain<X> & m, const KleisliMor & g)
      -> category::apply_kleisli_morph<chain, X, KleisliMor>
   {
      return category::apply_kleisli_morph<chain, X, KleisliMor>{ g(m.x) };
   };
};

/*

#include <iostream>

// chain がモナド測を満たすかどうかのテスト
// 適宜自作モナドのテスト用に書き換えてください

template <template <class> class M, typename T, typename U>
requires requires(T x, U y) { {x + y} -> std::same_as<U>; } // この例では、この演算を使っているため
constexpr bool monad_rule(T x, U) // monad がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY はクライスリ圏における射
   auto f = [](const T& x) -> M<T> { return {{},x}; };
   auto g = [](const T& x) -> M<U> { return {{},x * 3.14}; };

   bool hidari_id = (M<T>::ret(x) >> f) == f(x); // 左単位元律
   bool migi_id = (M<T>::ret(x) >> M<T>::ret) == M<T>::ret(x); // 右単位元律
      std::cout << std::boolalpha << hidari_id << migi_id;
   auto a = ( M<T>::ret(x) >> f ) >> g;
   auto b = M<T>::ret(x) >> ( [f, g](T x){
       return (f(x) >> g); } );
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

int main()
{
   chain<int> c{{},6};
   std::cout << (c >> [](int a){ return chain<int>{{},a+1}; }).x;
   c.fmap([](int a){ return a;});
   
   std::cout << std::boolalpha << monad_rule<chain>(14, 3.15);
}

*/

} // namespace monad_

using namespace monad_;

} // nyaruga :: util

#endif // NYARUGA_UTIL_MONAD_HPP
