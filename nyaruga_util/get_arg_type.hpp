
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_GET_ARG_TYPE_HPP
#define NYARUGA_UTIL_GET_ARG_TYPE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// このコードは、https://cdecrement.blog.fc2.com/blog-entry-234.html
// からいただいてきたたものです。

namespace nyaruga_util {

namespace nyaruga_util_impl
{

//テンプレートパラメータの指定した位置の型を取得
template<int N, typename... T>
struct get_template_param {
	//引数の数が足りない
	using type = void;
};
//テンプレートパラメータが一つ以上の時の部分特殊化
template<int N, typename First, typename... Rest>
struct get_template_param<N, First, Rest...> {
	using type = std::conditional_t<N == 0, First, 
	    typename get_template_param<N - 1, Rest...>::type>;
};

} // namespace nyaruga_util_impl

//関数の引数の型を得る
template<typename Func, int N>
struct get_function_argument_type {
};
//メンバ関数用特殊化
template<class C, typename Ret, typename... Args, int N>
struct get_function_argument_type<Ret(C::*)(Args...), N> {
	using type = typename nyaruga_util_impl::get_template_param<N-1, Args...>::type;
};
//通常関数用特殊化
template<typename Ret, typename... Args, int N>
struct get_function_argument_type<Ret(*)(Args...), N> {
	using type = typename nyaruga_util_impl::get_template_param<N-1, Args...>::type;
};

//get_function_argument_typeのエイリアステンプレート版
template<typename Func, int N>
using get_function_argument_type_t = typename get_function_argument_type<Func, N>::type;

} // namespace nyaruga_util

/*

class Test{
public:
	void memfunc(bool,int,double,std::string);
};

void func(bool,int,double,std::string);

int main() {
	std::cout << typeid(get_function_argument_type_t<decltype(&Test::memfunc),1>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&func),2>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&Test::memfunc),3>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&func),5>).name() << std::endl;
	return 0;
}

*/

#endif // #ifndef NYARUGA_UTIL_GET_ARG_TYPE_HPP