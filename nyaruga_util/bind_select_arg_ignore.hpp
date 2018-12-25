
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP
#define NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <type_traits>
#include <functional>
#include <nyaruga_util/make_compile_error.hpp>

namespace nyaruga_util {

namespace nyaruga_util_impl {

template<size_t current_pos, size_t target_pos, typename F, typename Head, typename ... Pack>
constexpr decltype(auto) 
bind_select_arg_ignore_impl(F && func, Head && head, Pack && ... pack) noexcept {
	if constexpr (current_pos == target_pos)
		return std::bind(std::forward<F>(func), std::forward<Pack>(pack)...);
	else if constexpr (current_pos < target_pos)
		return bind_select_arg_ignore_impl<current_pos + 1, target_pos> (
			[func = std::forward<F>(func), head = std::forward<Head>(head)]
		    (auto && ... args) { 
			    return func(head, args...); 
		    }, 
			std::forward<Pack>(pack)...
		);
	else
		NYARUGA_MAKE_COMPILE_ERROR("pos is out of pac");
}

} // namespace nyaruga_util_impl

   // パラメーターパックの任意の位置の引数を無視して、
	// その位置の型の引数をとる関数を返す高階関数
template<size_t pos, typename F, typename ... Pack>
constexpr decltype(auto) 
bind_select_arg_ignore(F && func, Pack && ... pack) noexcept {
	return nyaruga_util_impl::bind_select_arg_ignore_impl<1, pos>(
		std::forward<F>(func), std::forward<Pack>(pack)...);
}

} // namespace nyaruga_util

/*

using namespace nyaruga_util;

#include <iostream>

int main()
{
	auto ll = [](auto && ... hh)
	{
		(std::cout << ... << std::forward<decltype(hh)>(hh));
	};

	// 現在、可変長引数関数にしか対応してないです　↓　これにはつかえません
	// auto fn = [](int i, double d, float f, int h)
	// {
	//	   std::cout << i << " " << d << " " << f << " " << h;
	// };

	auto fn_ = bind_select_arg_ignore<3>(ll, 9, 10, 11);

	fn_(22); // 9, 11

}
*/

#endif // #ifndef NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP