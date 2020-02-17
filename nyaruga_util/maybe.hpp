//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MAYBE_HPP
#define NYARUGA_UTIL_MAYBE_HPP

#pragma once

#include <optional>
#include <compare>
#include <utility>
#include <nyaruga_util/category.hpp>

// std::optional をモナディックにしました
// ↑ このようなものは現在 C++ 標準化委員会に提案されています

namespace nyaruga::util {

namespace maybe_ {

static inline std::nullopt_t nothing = std::nullopt;

template <class T>
class just {
private: 
   T m_val;
public:
   constexpr just(const T & x) noexcept : m_val(x) {}
   constexpr just(T && x) noexcept : m_val(std::forward<T>(x)) {}
   template <std::convertible_to<T> U>
      requires std::is_copy_assignable_v<T>
   constexpr auto& operator = (const just<U>& other) noexcept(std::is_nothrow_assignable_v<T>) { T tmp = other.m_val; swap(m_val, tmp); return *this; }
   template <std::convertible_to<T> U>
      requires std::is_move_assignable_v<T>
   constexpr auto& operator = (just<U>&& other) noexcept(std::is_nothrow_move_assignable_v<T>) { m_val = std::move(other.m_val); return *this; }
   constexpr auto& unwrap() noexcept { return m_val; }
   constexpr auto operator <=> (const just&) const noexcept = default;
};

template <typename T>
struct maybe : public std::optional<just<T>> {
   using std::optional<just<T>>::optional;
   constexpr bool friend operator==(const maybe & m, const maybe & other) noexcept {
      return static_cast<std::optional<just<T>>>(m) == static_cast<std::optional<just<T>>>(other);
   };
   constexpr auto& unwrap() { return this->value().unwrap(); }
};

template <typename T>
maybe(T) -> maybe<T>;

template <typename T>
maybe(just<T>) -> maybe<T>;

template <class T, class F>
constexpr auto operator>=(const std::optional<just<T>> & x, F && f) noexcept(noexcept(f(x.value().unwrap())))
   -> maybe<category::apply_mu<maybe, decltype(f(x.value().unwrap()))>>
   requires category::kleisli_morphism_from<F, maybe, T>
{
   return (x.has_value()) ? f(x.value().unwrap()) : maybe<category::apply_mu<maybe, decltype(f(x.value().unwrap()))>>(std::nullopt);
}

template <class T, class F>
constexpr auto operator>=(maybe<T> && x, F && f) noexcept(noexcept(f(std::move(x.value().unwrap()))))
   -> maybe<category::apply_mu<maybe, decltype(f(std::move(x.value().unwrap())))>>
   requires category::kleisli_morphism_from<F, maybe, T>
{
   return (x.has_value()) ? f(std::move(x.value().unwrap())) : maybe<category::apply_mu<maybe, decltype(f(std::move(x.value().unwrap())))>>(std::nullopt);
}

template <class T>
constexpr auto ret(T&& x) noexcept -> decltype(maybe(std::forward<T>(x)))
{
   return maybe(std::forward<T>(x));
}

template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, maybe>
constexpr auto operator|(const maybe<T>& x, Func && f) noexcept(noexcept(f(x.value().unwrap())))
{
   return x.has_value() ? maybe<category::apply_morphism<T, Func>>{ f(x.value().unwrap()) } : maybe<category::apply_morphism<T, Func>>{ nothing };
}
   
template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, maybe>
constexpr auto operator|(maybe<T>&& x, Func && f) noexcept(noexcept(f(std::move(x.value().unwrap()))))
{
   return x.has_value() ? maybe<category::apply_morphism<T, Func>>{ f(std::move(x.value().unwrap())) } : maybe<category::apply_morphism<T, Func>>{ nothing };
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


// テストその２

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

// monad がモナド則を満たすかどうかのテスト
template <template <class> class M, typename T, typename U>
constexpr bool monad_rule(T x, U) 
{
   // f, g : X -> TY は任意
   auto f = [](const T&) -> M<T> { std::cout << "callf:constructed "; return just{ copy_move_test(0) }; };
   auto g = [](const T&) -> M<U> { std::cout << "ccallg:onstructed "; return just{ copy_move_test(3.14) }; };

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
   maybe<copy_move_test> {6} >= [](const copy_move_test&) { return maybe<copy_move_test>{ just(copy_move_test(1)) }; };
   maybe<copy_move_test> {6} | [](const copy_move_test&) { return copy_move_test(1); };

   std::cout << std::boolalpha << monad_rule<maybe>(copy_move_test(14), copy_move_test(3.15));

   std::cout << "total_construct: " << total_construct << " total_copyed: " << total_copy << " total_moved: " << total_move;
}

// ベンチマークテスト

#include <concepts>
#include <iostream>
#include <nyaruga_util/check_time.hpp>
#include <nyaruga_util/copy_move_test.hpp>
#include <nyaruga_util/maybe.hpp>

using namespace nyaruga::util;

// maybe がモナド則を満たすかどうかのテスト
template <template <class> class M, typename T, typename U>
constexpr bool monad_rule(T&& x, U&&)
{
   // f, g : X -> TY は任意
   auto f = [](T&&) -> M<T> { return { copy_move_test(0) }; };
   auto g = [](T&&) -> M<U> { return { copy_move_test(3.14) }; };

   bool hidari_id = (ret(std::forward<T>(x)) >= f) == f(std::forward<T>(x)); // 左単位元律
   bool migi_id = (ret(std::forward<T>(x)) >= ret<T>) == ret(std::forward<T>(x)); // 右単位元律
   auto a = (ret(std::forward<T>(x)) >= f) >= g;
   auto b = ret(std::forward<T>(x)) >= ([f, g](T&& x) {
      return (f(std::forward<T>(x)) >= g); });
   bool ketugou = a == b;  // 結合律
   return hidari_id && migi_id && ketugou;
}

int main()
{
   int aaa, bbb;
   std::cin >> aaa >> bbb;
   {
      maybe<copy_move_test> c{aaa};
      auto a = check_time();
      int bb = 10000000;
      
      while(bb --> 0) {
         c = std::move(maybe<copy_move_test> {aaa} 
            >= [aaa](copy_move_test&& b) { return  aaa == 0 ? maybe<copy_move_test>{nothing} : maybe<copy_move_test>{ std::move(b) }; }
            >= [bbb](copy_move_test&& b) { return  bbb == 0 ? maybe<copy_move_test>{nothing} : maybe<copy_move_test>{ std::move(b) }; }
            >= [aaa, bbb](copy_move_test&& b) { return  aaa * bbb != 15515  ? maybe<copy_move_test>{ std::move(b) } : maybe<copy_move_test>{ nothing }; });
      }
      
      print_copy_move_and_reset();
      std::cout << (c.has_value() ? c.unwrap().hello() : "tmp");
   }

   {
      std::optional<copy_move_test> c;
      auto a = check_time();
      int bb = 10000000;
      
      while(bb --> 0) {
         std::optional<copy_move_test> a{aaa};
         if (a.has_value())
         {
            auto f =  [aaa](copy_move_test&& b) { return  aaa == 0 ? std::optional<copy_move_test>{nothing} : std::optional<copy_move_test>{ std::move(b) }; };
            auto d = std::move(f(a));
            if (d.has_value())
            {
               auto g = [bbb](copy_move_test&& b) { return  bbb == 0 ? std::optional<copy_move_test>{nothing} : std::optional<copy_move_test>{ std::move(b) }; };
               auto r = std::move(g(d));
               if (r.has_value())
                  c = std::move([aaa, bbb](copy_move_test&& b) { return  aaa * bbb != 15515  ? std::optional<copy_move_test>{ std::move(b) } : std::optional<copy_move_test>{ nothing }; }(std::move(r)));
            }
         }
      }
      print_copy_move_and_reset();
      std::cout << (c.has_value() ? c.value().hello() : "tmp");
   }
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
