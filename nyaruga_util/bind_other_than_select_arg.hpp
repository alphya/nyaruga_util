
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP
#define NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <type_traits>

namespace nyaruga_util {

namespace nyaruga_util_impl {

template<std::size_t current_pos, std::size_t arg_pos, typename F, typename H, typename ... Arg>
decltype(auto) bind_other_than_select_arg_impl(F && f, H && head, Arg && ... arg)
{
	if constexpr (current_pos < arg_pos)
		return bind_other_than_select_arg_impl<current_pos + 1, arg_pos>(
			[f = std::forward<decltype(f)>(f), head = std::forward<decltype(head)>(head)]
        	(auto && ... args) {
		        return f(head, std::forward<decltype(args)>(args)...);
			}, std::forward<decltype(arg)>(arg)...);

	else if constexpr (current_pos == arg_pos)
		return [](auto && f, auto && head, auto && ... args) {
			return std::bind(std::forward<decltype(f)>(f),
				std::placeholders::_1, std::forward<decltype(head)>(head),
				std::forward<decltype(args)>(args)...);
			} (std::forward<decltype(f)>(f), std::forward<decltype(head)>(head),
				std::forward<decltype(arg)>(arg)...);
			else
				static_assert(std::bool_constant<false>, "This function does not has requests argument.");
}

}  // namespace nyaruga_util_impl

	// 指定した引数以外を適用した関数を返す高階関数 f(arg_pos = 1, 2, 3, 4...)
template<std::size_t arg_pos, typename F, typename ... Arg>
decltype(auto) bind_other_than_select_arg(F && f, Arg && ... arg) {
	return nyaruga_util_impl::bind_other_than_select_arg_impl<1, arg_pos>
		(std::forward<F>(f), std::forward<Arg>(arg)...);
}

}  // namespace nyaruga_util

/*


#include <iostream>

int main()
{

	auto fn = [](int i, double d, float f, int h)
	{
		std::cout << i << " " << d << " " << f << " " << h;
	};

	// fn(66.f, <placeholder>, 89.f, 47)
	//    1     2              3     4   <- template arg
	//          ↑
	auto && aaa = nyaruga_util::bind_other_than_select_arg<2>(fn, 66.f, 89.f, 47);

	// 66 999 89 47
	aaa(999.f);

}
*/

#endif // #ifndef NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP