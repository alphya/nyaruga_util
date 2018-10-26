#include <iostream>
#include <string>

#include "../../../nyaruga_util/runtime_unpac.hpp"

int main() {
	using namespace nyaruga_util;

	std::vector<int> v{ 10, 20, 30 };

	const auto lambda = [](auto&& ... args)
	{
		return (args + ... + 0);
	};

	std::cout << runtime_unpack<10>(lambda, v)();

}


