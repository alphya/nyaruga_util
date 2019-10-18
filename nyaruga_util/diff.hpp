
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

// 関数 f の、点 x での微分係数を返します
// 7 点近似です
template <typename F, typename Arg, typename Number = num_t>
Number diff(F && f, Arg && x) noexcept(noexcept(f(x)))
{
   auto h = DBL_EPSILON;
   auto&& y1 = f(static_cast<Number>(x + h));
   auto&& y2 = f(static_cast<Number>(x - h));
   auto&& y3 = f(static_cast<Number>(x + 2 * h));
   auto&& y4 = f(static_cast<Number>(x - 2 * h));
   auto&& y5 = f(static_cast<Number>(x + 3 * h));
   auto&& y6 = f(static_cast<Number>(x - 3 * h));

   return (y5 - 9 * y3 + 45 * y1 - 45 * y2 + 9 * y4 - y6) /
                          (60 * h);
}

// 5 点近似です
template <typename F, typename Arg, typename Number = num_t>
Number diff_fast(F && f, Arg && x) noexcept(noexcept(f(x)))
{
   auto && h = DBL_EPSILON;
   auto && y1 = f(x + h);
   auto && y2 = f(x - h);
   auto && y3 = f(x + 2 * h);
   auto && y4 = f(x - 2 * h);

   return (y4 - 8 * y2 + 8 * y1 - y3) / (12 * h);
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_DIFF_HPP