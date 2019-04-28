
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_DARK_OVERRODE_HPP
#define NYARUGA_UTIL_DARK_OVERRODE_HPP

#pragma once

#include <type_traits>

namespace nyaruga {

namespace util {

namespace stream_op_overloaded {

// operator >> のオーバーロード
template <typename T, typename F>
constexpr inline auto operator>>(T && arg,
                                 F && func) noexcept(noexcept(func(arg))) ->
   typename std::invoke_result_t<F, T>
{
   return func(std::forward<T>(arg));
}

// operator << のオーバーロード
template <typename F, typename T>
constexpr inline auto operator<<(F && func,
                                 T && arg) noexcept(noexcept(func(arg))) ->
   typename std::invoke_result_t<F, T>
{
   return func(std::forward<T>(arg));
}

} // namespace stream_op_overloaded

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_DARK_OVERRODE_HPP