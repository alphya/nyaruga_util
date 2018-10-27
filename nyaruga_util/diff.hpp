#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <limits>
#include <type_traits>
#include "get_arg_type.hpp"

namespace nyaruga_util {

// ������1,000�r�b�g�������������_���̒�`�B���Ȃ肷����
using num_t = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000>>;

// 7�_�ߎ� 5�_��萸�x����
template <typename T = num_t, typename F, typename Arg>
decltype(auto) diff(F && f, Arg && x)
{
	using result_type = std::invoke_result_t<F, Arg>;
	using arg_type = get_function_argument_type_t<decltype(&f), 0>;

	T&& h = LDBL_EPSILON;
	T&& y1 = f(static_cast<arg_type>(static_cast<T>(x) + h));
	T&& y2 = f(static_cast<arg_type>(static_cast<T>(x) - h));
	T&& y3 = f(static_cast<arg_type>(static_cast<T>(x) + 2 * h));
	T&& y4 = f(static_cast<arg_type>(static_cast<T>(x) - 2 * h));
	T&& y5 = f(static_cast<arg_type>(static_cast<T>(x) + 3 * h));
	T&& y6 = f(static_cast<arg_type>(static_cast<T>(x) - 3 * h));

	return static_cast<result_type>((std::forward<T>(y5) - 9 * std::forward<T>(y3)
		+ 45 * std::forward<T>(y1) - 45 * std::forward<T>(y2) + 9 *
		std::forward<T>(y4) - std::forward<T>(y6)) / (60 * std::forward<T>(h)));
}

// 5�_�ߎ�
template <typename T = num_t, typename F, typename Arg>
decltype(auto) diff_5(F && f, Arg&& x)
{
	using result_type = std::invoke_result_t<F, Arg>;
	using arg_type = get_function_argument_type_t<decltype(&f), 0>;

	T&& h = LDBL_EPSILON;
	T&& y1 = f(static_cast<arg_type>(static_cast<T>(x) + h));
	T&& y2 = f(static_cast<arg_type>(static_cast<T>(x) - h));
	T&& y3 = f(static_cast<arg_type>(static_cast<T>(x) + 2 * h));
	T&& y4 = f(static_cast<arg_type>(static_cast<T>(x) - 2 * h));

	return static_cast<result_type>((std::forward<T>(y4) - 8 *
		std::forward<T>(y2) + 8 * std::forward<T>(y1) -
		std::forward<T>(y3)) / (12 * std::forward<T>(h)));
}

}  // nyaruga_util

/*

using namespace nyaruga_util;

// ��������֐���`�B
// ���x�̂��߁Adouble�Ȃǂł͂Ȃ�nyaruga_util::num_t���g���Ă�������
num_t fn(num_t && x)
{
	return (x*x*x);
}

int main()
{

	num_t d = diff(fn, 3.f);

	std::cout << std::setprecision(std::numeric_limits<num_t>::digits10 + 1)
		<< "���҂����l�F27" << "\ndiff : " << d << "\ndelta: " << LDBL_EPSILON;

}


*/