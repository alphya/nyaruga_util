//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MAYBE_HPP
#define NYARUGA_UTIL_MAYBE_HPP

#pragma once

#include <nyaruga_util/category.hpp>

// std::optional をモナディックにしました
// ↑ このようなものは現在 C++ 標準化委員会に提案されています

namespace nyaruga::util {

#include <optional>

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
   maybe a = just{3};
   auto b = a | [](int a) { return a + 1; } | [](int a) { return a * 3; };

   std::cout << std::boolalpha << monad_rule<maybe>(14, 3.15) << b.value().unwrap();
}
*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_MAYBE_HPP
