
/*!
 * @file 
 * @brief 
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP
#define NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP

#pragma once

#include <type_traits>

namespace nyaruga::util::arg_is_func_for_for_each {
constexpr auto _1 = [](auto && f) { return std::forward<decltype(f)>(f)(); };
} // namespace nyaruga::util::arg_is_func_for_for_each

#endif // #ifndef NYARUGA_UTIL_FUNCTA_FOR_FUNC_HPP