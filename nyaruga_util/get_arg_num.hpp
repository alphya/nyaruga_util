#pragma once

namespace nyaruga_util {

//’ÊíŠÖ”—p
template<typename Ret, typename... Args>
static constexpr decltype(auto) get_function_argument_num(Ret(*)(Args...)) {
	return sizeof...(Args);
};

//ƒƒ“ƒoŠÖ”—p
template<typename Ret, typename C, typename... Args>
static constexpr decltype(auto) get_function_argument_num(Ret(C::*)(Args...)) {
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