#pragma once

#include "../../../nyaruga_util/diff.hpp"
#include "../../../nyaruga_util/bind_other_than_any_pos_arg.hpp"
#include "../../../nyaruga_util/get_arg_num.hpp"
#include "../../../nyaruga_util/pac_select_pos_obj.hpp"

namespace nyaruga_util
{

// 誤差を含む値
struct value_including_error
{
	constexpr value_including_error() = default;
	num_t value;  // 値
	num_t error;  // 誤差
};

namespace nyaruga_util_impl {

// 偏微分^2 * 誤差^2 を、一変数分だけ計算して、再帰
template<std::size_t count, std::size_t max, typename F, typename T, typename ... Args>
num_t generate_error_impl(F && f, Args && ... args)
{
	if constexpr (count > max) return num_t{0};

	num_t partial_diff;
	num_t current_val_obj = pac_select_pos_obj<count>(args...); 
	num_t current_val_error = current_val_obj.error;

	partial_diff = diff(bind_other_than_any_pos_arg<count>(f, args), current_val_obj.value);

	num_t result = pow(partial_diff, 2) * pow(current_val_error, 2);
	return result + generate_error_impl<count + 1, get_function_argument_num(f)>
		(std::forward<F>(f), std::forward<Args>(args)...);
}

}  // nyaruga_util_impl

// 誤差の伝播式より、計算
// Args には、value_including_errorを使用してください
template<typename F, typename ... Args> 
num_t generate_error(F && f, Args && ... args)
{
	return boost::multiprecision::pow(nyaruga_util_impl::generate_error_impl<1, get_function_argument_num(f)>
		(std::forward<F>(f), std::forward<Args>(args)...), 0.5l);
}


}  // nyaruga_util

