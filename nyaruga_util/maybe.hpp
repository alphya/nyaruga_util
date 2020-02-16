//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MAYBE_HPP
#define NYARUGA_UTIL_MAYBE_HPP

#pragma once

#include <optional>
#include <nyaruga_util/category.hpp>

// std::optional をモナディックにしました
// ↑ このようなものは現在 C++ 標準化委員会に提案されています

namespace nyaruga::util {

namespace maybe_ {

static inline std::nullopt_t nothing = std::nullopt;

template <class T>
class just {
public:
   T val;
   constexpr just(const T & x) noexcept : val(x) {}
   constexpr just(T && x) noexcept : val(std::forward<T>(x)) {}
   constexpr friend bool operator==(const just & lhs, const just & rhs) noexcept { return lhs.val == rhs.val; }
   constexpr decltype(auto) unwrap () const noexcept { return val; }
};

template <typename T>
using maybe = std::optional<just<T>>;

template <class T, class F>
constexpr auto operator>=(const std::optional<just<T>> & x, const F & f) noexcept 
   -> std::optional<just<category::apply_mu<maybe, decltype(f(x.value().val))>>>
{
   return (x.has_value()) ? f(x.value().val) : std::optional<just<category::apply_mu<maybe, decltype(f(x.value().val))>>>(std::nullopt);
}

template <class T>
constexpr std::optional<just<T>> ret(T x) noexcept
{
   return just<T>(x);
}

template <typename T, typename Func>
constexpr auto operator|(const maybe<T>& x, const Func & f) noexcept
{
   return x.has_value() ? maybe<category::apply_morphism<T, Func>>{ f(x.value().val) } : maybe<category::apply_morphism<T, Func>>{ nothing };
}

// テストの例
// MSCV ではなぜか失敗します...
// static_assert(category::monad<maybe, int, double>);
// static_assert(category::monad<maybe, float, std::string>);

} // namespace maybe_another_

using namespace maybe_;

/*

// maybe がモナド測を満たすかどうかのテスト

template <template <class> class M, typename T, typename U>
requires requires(T x, U y) { {x + y}->std::same_as<U>; } // この例では、この演算を使っているため
constexpr bool monad_rule(T x, U) // maybe がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY は任意
   auto f = [](const T& x) -> M<T> { return { x }; };
   auto g = [](const T& x) -> M<U> { return { x * 3.14 }; };

   bool hidari_id = (ret(x) >= f) == f(x); // 左単位元律
   bool migi_id = (ret(x) >= ret<T>) == ret(x); // 右単位元律
   auto a = (ret(x) >= f) >= g;
   auto b = ret(x) >= ([f, g](T x) {
      return (f(x) >= g); });
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

int main()
{
   maybe<int>{10} >= [](int a) { return just{ a + 1 }; } >= [](int a) { return just{ a + 2 }; };
   maybe<int>{nothing} >= [](int a) { return just{ a + 1 }; } >= [](int a) { return just{ a + 2 }; };
   maybe<int> a = just{3};
   auto b = a | [](int a) { return a + 1; } | [](int a) { return a * 3; };

   std::cout << std::boolalpha << monad_rule<maybe>(14, 3.15) << b.value().unwrap();
}
*/


// maybe モナドをクラスで作ったときの例


/*

// CRTP でこれを継承してエラーが出ないもので、下記のモナド則を満たすものがモナド
// nyaruga_util/category では、(static) メンバ関数 ret : T -> monad<T>, fmap : (X -> Y) -> (monad<X> -> mondo<Y>),
// operator >= : 引数が monad<X>, f : X -> monad<Y> で戻り値が monad<Y> の 3 種類のメンバを使ってモナドとしている
template <template <class> class T, typename X, typename Y = X>
struct monad {
   monad() { static_assert(category::monad<T, X, Y>, "The class is not a monad."); }
};

template <typename X>
struct meybe : monad<meybe, X>, public std::optional<X> {
   // 新しいモナドを作るときは、この部分と
   template <typename Type>
   using self = meybe<Type>;

   using std::optional<X>::optional;

   // この部分と
   static auto ret(X x) { return self<X>{ x }; }; // η. Haskell のモナドの型クラスにおける return

   // これは、category::functor を満たすようにするためのもので、直接は関係ない
   template <category::morphism_from<X> Mor>
   auto fmap(const Mor & f) noexcept
   {
      return [f, this](const self<X> & c) noexcept {
         // この部分と
         return this->has_value() ? self<category::apply_morphism<X, Mor>>{
            f(c.value())
         }
                                  : self<category::apply_morphism<X, Mor>>{ std::nullopt };
      };
   }

   constexpr bool friend operator==(const self<X> & m, const self<X> & other) noexcept
   {
      // この部分と
      return static_cast<std::optional<X>>(m) == static_cast<std::optional<X>>(other);
   };

   // これはあってもなくても良い
   template <category::morphism_from<X> Mor>
   requires requires(Mor f, X x)
   {
      {
         f(x)
      }
      ->category::same_T_rank_with<self, X>;
   }
   constexpr auto friend operator|(const self<X> & m, const Mor & g) noexcept
   {
      // この部分と
      return m.has_value() ? g(m.value()) : self<category::apply_morphism<X, Mor>>{ std::nullopt };
   }

   // Haskell における >>=
   template <category::kleisli_morphism_from<self, X> KleisliMor>
   // fmap がいらないときは、ここを削除する
   requires category::functor<self, X, category::apply_mu<self, category::apply_kleisli_morph<self, X, KleisliMor>>> constexpr auto friend operator>=(const self<X> & m, const KleisliMor & g) noexcept
      -> category::apply_kleisli_morph<self, X, KleisliMor>
   {
      // この部分を書き換える
      return m.value() ? category::apply_kleisli_morph<self, X, KleisliMor>{ g(m.value()) }
                       : category::apply_kleisli_morph<self, X, KleisliMor>{ std::nullopt };
   };
};


// maybe がモナド則を満たすかどうかのテスト
template <template <class> class M, typename T, typename U>
requires requires(T x, U y) { {x + y} -> std::same_as<U>; } // この例では、この演算を使っているため
constexpr bool monad_rule(T x, U) // monad がモナドであることを確かめる。モナド則を満たしていることを確認する
{
   // f, g : X -> TY はクライスリ圏における射
   auto f = [](const T& x) -> M<T> { return {x}; };
   auto g = [](const T& x) -> M<U> { return {x * 3.14}; };

   bool hidari_id = (M<T>::ret(x) >= f) == f(x); // 左単位元律
   bool migi_id = (M<T>::ret(x) >= M<T>::ret) == M<T>::ret(x); // 右単位元律
   auto a = ( M<T>::ret(x) >= f ) >= g;
   auto b = M<T>::ret(x) >= ( [f, g](T x){
      return (f(x) >= g); } );
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

int main()
{

   meybe<int> c{6};
   std::cout << (c >= [](int a){ return (a == 6)? meybe<int>{std::nullopt} : meybe{a+1}; }).has_value();
   c.fmap([](int a){ return a;});
   std::cout << (c | [](int a){ return a+1; }).value();

   std::cout << std::boolalpha << monad_rule<meybe>(14, 3.15);
}
*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_MAYBE_HPP
