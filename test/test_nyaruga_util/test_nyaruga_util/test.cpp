#include <iostream>

#include "../../../nyaruga_util/diff.hpp"



#include <functional>
#include <type_traits>
#include "../../../nyaruga_util/false_v.hpp"


namespace nyaruga_util {


template<std::size_t current_pos, std::size_t arg_pos, typename F, typename H, typename ... Arg>
decltype(auto) bind_other_than_any_pos_arg_impl(F && f, H && head, Arg && ... arg)
{
	if constexpr (current_pos < arg_pos)
	{
		return bind_other_than_any_pos_arg_impl<current_pos + 1, arg_pos>(
			[=]//[f = std::forward<F>(f), head = std::forward<decltype(head)>(head)]
		(auto && ... args)
		{ 
			return f(std::forward<decltype(head)>(head), std::forward<decltype(args)>(args)...); 
		}, 
			std::forward<decltype(arg)>(arg)...);
	}
	else if constexpr (current_pos == arg_pos)
	{
		return [f = std::forward<F>(f), head = std::forward<H>(head)] (auto && ... args)
		{ 
			return [=]// [f = std::forward<F>(f), head = std::forward<H>(head), args = std::forward<Arg>(args)]
			(auto&& any_pos_arg) 
			{ 
				return f(std::forward<decltype(head)>(head), any_pos_arg, std::forward<decltype(args)>(args)...);
			}; 
		}
		(std::forward<decltype(arg)>(arg)...);
	}
	else
		static_assert(false_v<F>, "out of arg num");
}

// 指定した引数以外を適用した関数を返す高階関数 f(arg_pos = 1, 2, 3, 4...)
template<std::size_t arg_pos, typename F, typename ... Arg>
decltype(auto) bind_other_than_any_pos_arg(F && f, Arg && ... arg)
{
	return bind_other_than_any_pos_arg_impl<1, arg_pos>(std::forward<F>(f), std::forward<Arg>(arg)...);
}

}  // nyaruga_util


int main()
{

	auto fn = [](int i, double d, float f, int h)
	{
		std::cout << i << " " << d << " " << f << " " << h;
	};

	// 第1引数のみを先に渡す
	using namespace std::placeholders;
	auto ninjin = std::bind(fn, 45, _1);
	auto Pike = std::bind(ninjin, 99.f, _1);
	// Pike(123, 100);

	auto && aaa = nyaruga_util::bind_other_than_any_pos_arg<2>(fn, 66.f, 89.f, 47);

	aaa(999.f);

}










