
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_DEF_HPP
#define NYARUGA_UTIL_DEF_HPP

#pragma once

#include <utility>

// 関数を bind します

namespace nyaruga {

namespace util {

namespace def_ {

namespace detail
{
   static inline auto l = [](auto&& x) { return x; };
}

template <typename F = decltype(detail::l)>
struct def
{
   F call;
   
   template <typename T>
   auto operator*(T&& f) {
      auto l = [g = std::forward<T>(f), f = this->call](auto&&... x)
      { 
         return f(g(std::forward<decltype(x)>(x)...));
      };
      return def<decltype(l)>{l};
   }

   template <typename...Args>
   decltype(auto) operator()(Args&&...args)
   {
      return call(std::forward<Args>(args)...);
   }
};

template <typename F>
def(F)->def<F>;

} // namespace def_

using namespace def_;

} // namespace nyaruga::util

/* 使い方

#include <concepts>
#include <iostream>
#include <boost/lambda/lambda.hpp>

using namespace nyaruga::util;
using boost::lambda::_1;

int main()
{
   auto fun = def{} * (_1 - 19) * (_1 + 20) * (_1 / 100) * (_1 * 100);
   std::cout << fun(100); // 201
}

*/

#endif // NYARUGA_UTIL_DEF_HPP
