
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP
#define NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP

#pragma once

#include <nyaruga_util/partial_diff.hpp>
#include <nyaruga_util/diff.hpp>
#include <boost/hana/functional/arg.hpp>

namespace nyaruga {

namespace util {

// 誤差を含む値
struct value_including_error {
   num_t value; // 値
   num_t error; // 誤差
   operator num_t() { return value; }
   operator num_t() const { return value; }
};

namespace nyaruga_util_impl {

// 偏微分^2 * 誤差^2 を、一変数分だけ計算して、再帰
template <std::size_t count, std::size_t max, typename F, typename... Args>
decltype(auto) generate_error_impl(F && f, Args &&... args)
{
   if constexpr (count > max)
      return num_t{ 0 };
   else {
      auto && current_val_obj = boost::hana::arg<count>(args...);
      auto && current_val_error = current_val_obj.error;
      auto && partial_diff = nyaruga::util::partial_diff<count>(f)(static_cast<num_t>(args)...);
      auto && result = boost::multiprecision::pow(partial_diff, 2) *
                       boost::multiprecision::pow(current_val_error, 2);
      return static_cast<num_t>(result + generate_error_impl<count + 1, max>(std::forward<F>(f), std::forward<Args>(args)...));
   }
}

} // namespace nyaruga_util_impl

// 誤差の伝播式より、計算。誤差を返す
// Args には、value_including_errorを使用してください
template <typename F, typename... Args>
decltype(auto) generate_error(F && f, Args &&... args)
{
   return static_cast<num_t>(boost::multiprecision::pow(
      nyaruga_util_impl::generate_error_impl<1, sizeof...(args)>(std::forward<F>(f), std::forward<decltype(args)>(args)...), 0.5l));
}

} // namespace nyaruga_util

/*
#include "value_including_error.hpp"
#include <iostream>
auto main() -> int
{
   // 誤差を含む値の定義 { 値, 誤差 }
   auto&& x = nyaruga::util::value_including_error{ 20, 0.5 };
   auto&& y = nyaruga::util::value_including_error{ 50, 0.8 };
   // 誤差を含む値の関数定義　これは単純な掛け算
   decltype(auto) f = [](nyaruga::util::num_t x, nyaruga::util::num_t y) {
      return static_cast<nyaruga::util::num_t>(x * y);
   };
   // 普通に関数を実行した値
   auto&& v = f(x, y);
   // 誤差を計算
   auto&& e = nyaruga::util::generate_error(f, x, y);
   // だいたいあってる程度の値です
   // 単純な掛け算で、有効数字17桁くらいでした
   std::cout << std::setprecision(18)
      << "値は：" << v << "\n誤差は：" << e << "\n";
}
*/

#endif // #ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP