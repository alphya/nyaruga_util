#pragma once

namespace nyaruga_util {

//�ʏ�֐��p
template<typename Ret, typename... Args>
constexpr decltype(auto) get_function_argument_num(Ret(*)(Args...)) noexcept 
{
	return sizeof...(Args);
};

//�����o�֐��p
template<typename Ret, typename C, typename... Args>
constexpr decltype(auto) get_function_argument_num(Ret(C::*)(Args...)) noexcept 
{
	return sizeof...(Args);
};

}

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