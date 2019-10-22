//              Copyright (c) 2019 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_PARTIAL_DIFF_HPP
#define NYARUGA_UTIL_PARTIAL_DIFF_HPP

#pragma once

#include <concepts>
#include <type_traits>
#include <nyaruga_util/forward_wrap.hpp>
#include <nyaruga_util/diff.hpp>
#include <nyaruga_util/bind_select_arg_replace.hpp>
#include <boost/hana/functional/arg.hpp>

namespace nyaruga::util {

namespace hana = boost::hana;

// Partial differentiation
template <std::size_t count, typename NumType = num_t, typename F>
constexpr auto partial_diff(F && f) noexcept
{
   return [f](auto&& ... args) noexcept -> NumType
   {
      return diff<NumType>(bind_select_arg_replace<count>(forward(f), args...))
         (hana::arg<count>(forward(args)...));
   };
}

} // namespace nyaruga::util

/* usage
#include <iostream>
#include <nyaruga_util/partial_diff.hpp>

int main()
{
   auto lambda = [](auto ... a) { return static_cast<nyaruga::util::num_t>((a * ... )); };

   std::cout << std::setprecision(18) << nyaruga::util::partial_diff<1>(lambda)(nyaruga::util::num_t(2), 5., 6.);
}
*/

#endif // #ifndef NYARUGA_UTIL_PARTIAL_DIFF_HPP