
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_DIFF_HPP
#define NYARUGA_UTIL_DIFF_HPP

#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <cmath>
#include <limits>
#include <nyaruga_util/get_arg_type.hpp>
#include <type_traits>

namespace nyaruga {

namespace util {

using num_t =
   boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000>>;

// 微分します
// 7 点近似です
template <typename Number = num_t, typename F>
constexpr auto diff(F && f) noexcept
{
   return [f](auto && x) noexcept(noexcept(f(x))) -> Number {
      auto h = 1e-7;
      if constexpr (std::is_same_v<Number, num_t>)
         h = DBL_EPSILON;

      auto&& y1 = f(static_cast<Number>(x + h));
      auto&& y2 = f(static_cast<Number>(x - h));
      auto&& y3 = f(static_cast<Number>(x + 2 * h));
      auto&& y4 = f(static_cast<Number>(x - 2 * h));
      auto&& y5 = f(static_cast<Number>(x + 3 * h));
      auto&& y6 = f(static_cast<Number>(x - 3 * h));

      return (y5 - 9 * y3 + 45 * y1 - 45 * y2 + 9 * y4 - y6) /
                           (60 * h);
   };
}

// 5 点近似です
template <typename Number = num_t, typename F>
constexpr auto diff_fast(F && f) noexcept
{
   return [f](auto && x) noexcept(noexcept(f(x))) -> Number
   {
      auto h = 1e-7;
      if constexpr (std::is_same_v<Number, num_t>)
         h = DBL_EPSILON;

      auto && y1 = f(static_cast<Number>(x + h));
      auto && y2 = f(static_cast<Number>(x - h));
      auto && y3 = f(static_cast<Number>(x + 2 * h));
      auto && y4 = f(static_cast<Number>(x - 2 * h));

      return (y4 - 8 * y2 + 8 * y1 - y3) / (12 * h);
   };
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_DIFF_HPP