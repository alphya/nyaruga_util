#pragma once

#include "../../../nyaruga_util/diff.hpp"
//#include "../../../nyaruga_util/bind_other_than_any_pos_arg.hpp"
#include "../../../nyaruga_util/pac_select_pos_replace.hpp"
#include "../../../nyaruga_util/get_arg_num.hpp"
#include "../../../nyaruga_util/pac_select_pos_obj.hpp"

// #include <cmath>

namespace nyaruga_util
{

// 誤差を含む値
struct value_including_error
{
	num_t value;  // 値
	num_t error;  // 誤差
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
		auto && partial_diff = diff(pac_select_pos_replace<count>(f, args...),
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

#include "value_including_error.hpp"

#include <iostream>

using namespace nyaruga_util;
using val = nyaruga_util::value_including_error;


decltype(auto) f(num_t i, num_t j)
{
	return static_cast<num_t>(i * j);
};

int main()
{

	auto i = val{ 20, 0.5 };
	auto j = val{ 50, 0.8 };

	auto r = generate_error(f, i, j);

	std::cout << std::setprecision(18) << r << "\n";

	std::cout << std::setprecision(std::numeric_limits<num_t>::digits10 + 1) << r;

	std::cout << "\n期待される値 : " << std::pow(20*20*0.8*0.8+50*50*0.5*0.5, 0.5);

}

*/

