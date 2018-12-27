
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP
#define NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <nyaruga_util/diff.hpp>
#include <nyaruga_util/bind_select_arg_replace.hpp>
#include <nyaruga_util/get_arg_num.hpp>
#include <nyaruga_util/pac_select_pos_obj.hpp>

namespace nyaruga_util
{

namespace value_including_error_ { // protection from unintended ADL

// 誤差を含む値
struct value_including_error
{
	num_t value;  // 値
	num_t error;  // 誤差
	operator num_t() { return value; }
	operator num_t() const { return value; }
};

}

using value_including_error = value_including_error_::value_including_error;

namespace nyaruga_util_impl {

// 偏微分^2 * 誤差^2 を、一変数分だけ計算して、再帰
template<std::size_t count, std::size_t max, typename F, typename ... Args>
decltype(auto) generate_error_impl(F && f, Args && ... args)
{
	if constexpr (count > max) return num_t{0};
	else
	{
		auto && current_val_obj = pac_select_pos_obj<count>(args...);
		auto && current_val_error = current_val_obj.error;
		auto && partial_diff = diff(bind_select_arg_replace<count>(f, args...),
			current_val_obj.value);
		auto && result = boost::multiprecision::pow(partial_diff, 2) *
			boost::multiprecision::pow(current_val_error, 2);
		return static_cast<num_t>(result + generate_error_impl <count + 1, max>
			(std::forward<F>(f), std::forward<Args>(args)...));
	}
}

} // namespace nyaruga_util_impl

// 誤差の伝播式より、計算。誤差を返す
// Args には、value_including_errorを使用してください
template<typename F, typename ... Args> 
decltype(auto) generate_error(F && f, Args && ... args)
{
	return static_cast<num_t>(boost::multiprecision::pow(
		nyaruga_util_impl::generate_error_impl<1, sizeof...(args)>
		(std::forward<F>(f), std::forward<decltype(args)>(args)... ), 0.5l));
}

} // namespace nyaruga_util


/*

#include <iostream>
#include "value_including_error.hpp"

auto main() -> int
{
	// 誤差を含む値の定義 { 値, 誤差 }
	auto && x = nyaruga_util::value_including_error{ 20, 0.5 };
	auto && y = nyaruga_util::value_including_error{ 50, 0.8 };

	// 誤差を含む値の関数定義　これは単純な掛け算
	decltype(auto) f = [](nyaruga_util::num_t x, nyaruga_util::num_t y) {
		return static_cast<nyaruga_util::num_t>(x * y);
	};

	// 普通に関数を実行した値
	auto && v = f(x, y);

	// 誤差を計算
	auto && e = nyaruga_util::generate_error(f, x, y);

	// だいたいあってる程度の値です
	// 単純な掛け算で、有効数字17桁くらいでした
	std::cout << std::setprecision(18)
		<< "値は：" << v << "\n誤差は：" << e << "\n";

}


*/

#endif // #ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP
