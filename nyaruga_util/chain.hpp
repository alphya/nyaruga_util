//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CHAIN_HPP
#define NYARUGA_UTIL_CHAIN_HPP

#pragma once

#include <nyaruga_util/category.hpp>

// メソッドチェインができます

namespace nyaruga::util {

namespace chain_ {

template <class T>
struct chain {
   const T val;
   constexpr chain(const T & x) noexcept : val(x) {}
   constexpr chain(T && x) noexcept : val(std::forward<T>(x)) {}
   constexpr friend bool operator==(const chain & lhs, const chain & rhs) noexcept { return lhs.val == rhs.val; }
   constexpr decltype(auto) unwrap() const noexcept { return val; }
};

template <class T, class F>
   requires category::kleisli_morphism_from<F, chain, T>
constexpr auto operator>=(const chain<T> & x, const F & f) noexcept
{
   return f(x.val);
}

template <class T>
constexpr chain<T> ret(const T& x) noexcept
{
   return chain<T>(x);
}

template <class T, class F>
   requires category::morphism_from<F, T> && category::kleisli_object<category::apply_morphism<T, F>, chain>
constexpr auto operator|(const chain<T> & x, const F & f) noexcept -> chain<category::apply_morphism<T, F>>
{
   return f(x.val);
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

*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
