//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_UNWRAP_HELPER_IDX_HPP
#define NYARUGA_UTIL_UNWRAP_HELPER_IDX_HPP

#pragma once

#include <boost/type_index.hpp>

namespace nyaruga::util {

// unwrap_helper_idx
template<unsigned int i, class U, class... Args>
struct unwrap_helper_idx_getter: unwrap_helper_idx_getter<i-1, Args...>{};

template<class U, class... Args>
struct unwrap_helper_idx_getter<0, U, Args...>{ using type = U; };

template<unsigned int i, template<class...> class U, class... Args>
auto unwrap_helper_idx_impl(const U<Args...>&) -> unwrap_helper_idx_getter<i, Args...>;

template<unsigned int i, class... Args, class R>
auto unwrap_helper_idx_impl( R(*fn)(Args...) ) -> typename unwrap_helper_idx_getter<i, Args...>::type;

template<unsigned int i, class U>
using unwrap_helper_idx = typename decltype(unwrap_helper_idx_impl<i>(std::declval<U>()))::type;

} // nyaruga :: util

#endif // NYARUGA_UTIL_UNWRAP_HELPER_IDX_HPP
