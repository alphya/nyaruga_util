
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP
#define NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
#endif

#include <type_traits>

namespace nyaruga_util::arg_is_func_for_for_each {
constexpr auto _1 = [](auto && f) { return std::forward<decltype(f)>(f)(); };
} // namespace nyaruga_util::arg_is_func_for_for_each

#endif // #ifndef NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP