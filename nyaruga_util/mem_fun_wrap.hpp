
/*!
 * @file 
 * @brief 
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_MEM_FUN_WRAP_HPP
#define NYARUGA_UTIL_MEM_FUN_WRAP_HPP

#pragma once

#include <type_traits>

namespace nyaruga {

namespace util {

template <typename Ret, typename C, typename Obj, typename... Args>
constexpr auto mem_fun_wrap(Ret (C::*&& f)(Args...), Obj *&& obj) noexcept
{
   return [f = std::forward<decltype(f)>(f), obj = std::forward<decltype(obj)>(obj)](auto &&... args) mutable {
      return (obj->*f)(std::forward<decltype(args)>(args)...);
   };
}

template <typename Ret, typename C, typename Obj, typename... Args>
constexpr auto mem_fun_wrap(Ret (C::*&& f)(Args...), Obj && obj) noexcept
{
   return [f = std::forward<decltype(f)>(f), obj = std::forward<decltype(obj)>(obj)](auto &&... args) mutable {
      return (obj.*f)(std::forward<decltype(args)>(args)...);
   };
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_MEM_FUN_WRAP_HPP