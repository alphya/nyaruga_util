
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_AUTO_DIFF_HPP
#define NYARUGA_UTIL_AUTO_DIFF_HPP

#pragma once

#include <cmath>
#include <concepts>
#include <Eigen/Core>
#include <Eigen/Geometry>

// 二重数を用いた自動微分ライブラリ
// TODO: std::xx -> xx (using ADL)

namespace nyaruga {

namespace util {

namespace detail {

template <std::floating_point R = double>
constexpr auto make_init_value(int dim, int var_no) noexcept 
{
    Eigen::Matrix<R,Eigen::Dynamic,1> tmp = Eigen::Matrix<R,Eigen::Dynamic,1>::Zero(dim);
    tmp(var_no - 1) = 1.;
    return tmp; 
}

} // namespace detail


template <std::floating_point R = double>
struct var {
	using value_type = R;
	using diff_value_type = Eigen::Matrix<R,Eigen::Dynamic,1>;
	value_type val;
	diff_value_type dval;
	constexpr var() noexcept = default;
	constexpr var(value_type val_, size_t dim, size_t var_no) noexcept : val(val_), dval(detail::make_init_value(dim, var_no)) {}
	constexpr var(value_type val_, diff_value_type dval_) noexcept : val(val_), dval(dval_) {}
	friend constexpr auto operator<=>(const var&, const var&) noexcept = default;
};

template <std::floating_point R>
constexpr auto operator + (const var<R>& lhs, const var<R>& rhs) noexcept
{
	return var(lhs.val + rhs.val, static_cast<var<R>::diff_value_type>(lhs.dval + rhs.dval));
}

template <std::floating_point R>
constexpr auto operator - (const var<R>& lhs, const var<R>& rhs) noexcept
{
	return var(lhs.val - rhs.val, static_cast<var<R>::diff_value_type>(lhs.dval - rhs.dval));
}

template <std::floating_point R>
constexpr auto operator * (const var<R>& lhs, const var<R>& rhs) noexcept
{
	return var(lhs.val * rhs.val, static_cast<var<R>::diff_value_type>(rhs.val*lhs.dval + lhs.val*rhs.dval));
}

// rhs.val != 0
template <std::floating_point R>
constexpr auto operator / (const var<R>& lhs, const var<R>& rhs)
{
	return var(lhs.val / rhs.val, static_cast<var<R>::diff_value_type>((lhs.dval*rhs.val - lhs.val*rhs.dval)/std::pow(rhs.val,2)));
}

template <std::floating_point R>
constexpr auto sin(const var<R>& x) noexcept
{
	return var(std::sin(x.val), static_cast<var<R>::diff_value_type>(std::cos(x.val)*x.dval));
}

template <std::floating_point R>
constexpr auto cos(const var<R>& x) noexcept
{
	return var(std::cos(x.val), static_cast<var<R>::diff_value_type>(-x.dval*std::sin(x.val)));
}

template <std::floating_point R>
constexpr auto exp(const var<R>& x) noexcept
{
	return var(std::exp(x.val), static_cast<var<R>::diff_value_type>(x.dval*std::exp(x.val)));
}

// x.val > 0
template <std::floating_point R>
constexpr auto log(const var<R>& x)
{
	return var(std::log(x.val), static_cast<var<R>::diff_value_type>(x.dval/x.val));
}

// x.val != 0
template <std::floating_point R>
constexpr auto pow(const var<R>& x, float num)
{
	return var(std::pow(x.val,num), static_cast<var<R>::diff_value_type>(x.dval*num*std::pow(x.val, num-1)));
}

} // namespace utiil

} // namespace nyaruga


/* how to use 

#include "auto_diff.hpp"

int main() {

	var x1(3.1415926/3,2,1), x2(7.,2,2), y;
	
	y = x1*x2 + sin(x1)*x2;
	printf("%f, %f, %f\n", y.val, y.dval(0), y.dval(1));
}

*/

#endif // #ifndef NYARUGA_UTIL_AUTO_DIFF_HPP
