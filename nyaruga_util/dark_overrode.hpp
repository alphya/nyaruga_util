
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_DARK_OVERRODE_HPP
#define NYARUGA_UTIL_DARK_OVERRODE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
#endif

#include <type_traits>

namespace nyaruga_util {

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

} // namespace nyaruga_util

#endif // #ifndef NYARUGA_UTIL_DARK_OVERRODE_HPP