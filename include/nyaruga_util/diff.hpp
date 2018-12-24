
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_DIFF_HPP
#define NYARUGA_UTIL_DIFF_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <limits>
#include <cmath>
#include <type_traits>
#include <nyaruga_util/get_arg_type.hpp>

namespace nyaruga_util {

// 仮数部1,000ビットを持つ浮動小数点数の定義。かなりすごい
using num_t = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000>>;

// 微分します
// 7点近似 5点より精度高い
template <typename F, typename Arg>
decltype(auto) diff(F && f, Arg && x) noexcept(noexcept(f(x)))
{
	num_t&& h = DBL_EPSILON;
	num_t&& y1 = f(static_cast<num_t>(x + h));
	num_t&& y2 = f(static_cast<num_t>(x - h));
	num_t&& y3 = f(static_cast<num_t>(x + 2 * h));
	num_t&& y4 = f(static_cast<num_t>(x - 2 * h));
	num_t&& y5 = f(static_cast<num_t>(x + 3 * h));
	num_t&& y6 = f(static_cast<num_t>(x - 3 * h));

	return static_cast<num_t>((std::forward<num_t>(y5) - 9 * std::forward<num_t>(y3)
		+ 45 * std::forward<num_t>(y1) - 45 * std::forward<num_t>(y2) + 9 *
		std::forward<num_t>(y4) - std::forward<num_t>(y6)) / (60 * std::forward<num_t>(h)));
}

// 5点近似
template <typename F, typename Arg>
decltype(auto) diff_fast(F && f, Arg&& x) noexcept(noexcept(f(x)))
{
	auto && h = DBL_EPSILON;
	auto && y1 = f(x + h);
	auto && y2 = f(x - h);
	auto && y3 = f(x + 2 * h);
	auto && y4 = f(x - 2 * h);
	return (y4 - 8 * y2 + 8 * y1 - y3) / (12 * h);
}



}  // namespace nyaruga_util

/*


using namespace nyaruga_util;

// 微分する関数定義。
// 精度のため、doubleなどではなくnyaruga_util::num_tを使ってください
num_t fn(num_t && x)
{
	return (x*x*x);
}

int main()
{

	num_t d = diff(fn, 3.f);

	std::cout << std::setprecision(std::numeric_limits<num_t>::digits10 + 1)
		<< "期待される値：27" << "\ndiff : " << d << "\ndelta: " << DBL_EPSILON;

}


*/

#endif // #ifndef NYARUGA_UTIL_DIFF_HPP