
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_RUNTIME_UNPAC_HPP
#define NYARUGA_UTIL_RUNTIME_UNPAC_HPP

#pragma once

#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace nyaruga {

namespace util {

template <std::size_t sentinel, std::size_t count, typename R, typename F,
          typename Itr>
constexpr std::function<R()> runtime_unpack_impl(F && f, Itr && current_itr,
                                                 Itr && end_itr) noexcept
{
   if (current_itr == end_itr)
      return [f = std::forward<F>(f)] { return f(); };

   if constexpr (sentinel >= count) {
      return runtime_unpack_impl<sentinel, count + 1, R>(
         [car = *current_itr++, f = std::forward<F>(f)](auto &&... args) {
            return f(car, std::forward<decltype(args)>(args)...);
         },
         std::forward<Itr>(current_itr), std::forward<Itr>(end_itr));
   }
   else {
      std::cout << u8"引数が多すぎます。 " << sentinel
                << u8"個までの引数にしてください。\n";
      return [f = std::forward<F>(f)] { return f(); };
   }
}

template <std::size_t sentinel, typename F, typename Container>
constexpr decltype(auto) runtime_unpack(F && f, const Container & v) noexcept
//-> decltype(std::enable_if_t<std::is_invocable_v<F,
//decltype(*std::begin(v))>>(),
//	std::function<decltype(f(*std::begin(v)))()>())
{
   return runtime_unpack_impl<sentinel, 1, decltype(f(*std::begin(v)))>(
      std::forward<F>(f), std::begin(v), std::end(v));
}

} // namespace util
} // namespace nyaruga

/*
int main()
{
    std::vector<int> v{10, 20, 30};

    const auto lambda = [](auto && ... args)
    {
        return (0 + ... + std::forward<decltype(args)>(args));
    };

    std::cout << runtime_unpack<10>(lambda, v)();
}
*/

#endif // #ifndef NYARUGA_UTIL_RUNTIME_UNPAC_HPP