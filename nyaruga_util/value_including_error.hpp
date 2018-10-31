#pragma once

#include "diff.hpp"
#include "bind_select_arg_replace.hpp"
#include "get_arg_num.hpp"
#include "pac_select_pos_obj.hpp"

namespace nyaruga_util
{

// 誤差を含む値
struct value_including_error
{
	num_t value;  // 値
	num_t error;  // 誤差
	operator num_t() { return value; }
	operator num_t() const { return value; }
};

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

}  // nyaruga_util_impl

// 誤差の伝播式より、計算。誤差を返す
// Args には、value_including_errorを使用してください
template<typename F, typename ... Args> 
decltype(auto) generate_error(F && f, Args && ... args)
{
	return static_cast<num_t>(boost::multiprecision::pow(
		nyaruga_util_impl::generate_error_impl<1, sizeof...(args)>
		(std::forward<F>(f), std::forward<decltype(args)>(args)... ), 0.5l));
}


}  // nyaruga_util


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

