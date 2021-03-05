
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_UNWRAP_TEMPLATE_HPP
#define NYARUGA_UTIL_UNWRAP_TEMPLATE_HPP

#pragma once

namespace nyaruga {

namespace util {

// unwrap_template_and_apply
template<template <class...> class To, template <class...> class From, typename ... Args>
auto unwrap_template_and_apply_impl(const From<Args...>&) -> To<Args...>;

template <typename From, template <class...> class To>
using unwrap_template_and_apply = decltype(unwrap_template_and_apply_impl<To>(std::declval<From>()));

// unwrap_template_idx
template<unsigned int i, class U, class... Args>
struct unwrap_helper_idx_getter: unwrap_helper_idx_getter<i-1, Args...>{};

template<class U, class... Args>
struct unwrap_helper_idx_getter<0, U, Args...>{ using type = U; };

template<unsigned int i, template<class...> class U, class... Args>
auto unwrap_helper_idx_impl(const U<Args...>&) -> unwrap_helper_idx_getter<i, Args...>;

template<unsigned int i, class... Args, class R>
auto unwrap_helper_idx_impl( R(*fn)(Args...) ) -> typename unwrap_helper_idx_getter<i, Args...>::type;

template<unsigned int i, class U>
using unwrap_template_idx = typename decltype(unwrap_helper_idx_impl<i>(std::declval<U>()))::type;

} // nyaruga :: util

#endif // NYARUGA_UTIL_UNWRAP_TEMPLATE_HPP
