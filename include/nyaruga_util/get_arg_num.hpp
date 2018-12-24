
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_GET_ARG_NUM_HPP
#define NYARUGA_UTIL_GET_ARG_NUM_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace nyaruga_util {

//通常関数用
template<typename Ret, typename... Args>
constexpr decltype(auto) 
get_function_argument_num(Ret(*)(Args...)) noexcept 
{
	return sizeof...(Args);
};

//メンバ関数用
template<typename Ret, typename C, typename... Args>
constexpr decltype(auto) 
get_function_argument_num(Ret(C::*)(Args...)) noexcept 
{
	return sizeof...(Args);
};
	
	/*
	//通常関数用
template<typename Ret, typename... Args>
struct get_function_argument_num
{
    static const std::size_t value = sizeof...(Args);
};

template<typename Ret, typename... Args>
constexpr std::size_t get_function_argument_num_v = get_function_argument_num<Ret,Args ...>::value;
	*/

} // namespace nyaruga_util

/*

struct bbb {
	auto fn(int i, double d)
	{
		std::cout << i << " " << d;
	};
};

auto fn(int i, double d, float f, int h, double p)
{
	std::cout << i << " " << d << " " << f << " " << h << " " << p;
};

int main()
{
	bbb kkk();

	auto && aaa = get_function_argument_num(&bbb::fn);

	auto && www = get_function_argument_num(fn);

	std::cout << aaa << " " << www;

}

*/

#endif // #ifndef NYARUGA_UTIL_GET_ARG_NUM_HPP
