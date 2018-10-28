#pragma once

#include "../../../nyaruga_util/diff.hpp"
#include "../../../nyaruga_util/bind_other_than_any_pos_arg.hpp"
#include "../../../nyaruga_util/get_arg_num.hpp"
#include "../../../nyaruga_util/pac_select_pos_obj.hpp"

namespace nyaruga_util
{

// �덷���܂ޒl
struct value_including_error
{
	constexpr value_including_error() = default;
	num_t value;  // �l
	num_t error;  // �덷
};

namespace nyaruga_util_impl {

// �Δ���^2 * �덷^2 ���A��ϐ��������v�Z���āA�ċA
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

// �덷�̓`�d�����A�v�Z
// Args �ɂ́Avalue_including_error���g�p���Ă�������
template<typename F, typename ... Args> 
num_t generate_error(F && f, Args && ... args)
{
	return boost::multiprecision::pow(nyaruga_util_impl::generate_error_impl<1, get_function_argument_num(f)>
		(std::forward<F>(f), std::forward<Args>(args)...), 0.5l);
}


}  // nyaruga_util

