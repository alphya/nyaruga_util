
#include "value_including_error.hpp"

#include <iostream>

using namespace nyaruga_util;
using val = value_including_error;

constexpr auto f(val i, val j) { return i.value * j.value; };

int main()
{

	auto i = val{ 20, 0.5 };
	auto j = val{ 50, 0.8 };

	

	auto r = generate_error(f, i, j);

	std::cout << r;

}
