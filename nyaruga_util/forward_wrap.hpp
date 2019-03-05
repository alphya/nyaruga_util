
//              Copyright (c) 2019 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_FORWARD_WRAP_HPP
#define NYARUGA_UTIL_FORWARD_WRAP_HPP

#pragma once

namespace nyaruga {

namespace util {

// You can write forward without template argument.
template <typename T>
[[nodiscard]] constexpr T && forward(T && arg) noexcept
{
   return std::forward<decltype(arg)>(arg);
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_FORWARD_WRAP_HPP