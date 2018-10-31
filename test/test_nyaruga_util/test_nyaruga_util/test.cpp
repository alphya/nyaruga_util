
#include "../../../nyaruga_util/value_including_error.hpp"

#include <iostream>

using namespace nyaruga_util;
using val = nyaruga_util::value_including_error;


decltype(auto) f(num_t i, num_t j) 
{ 
	return static_cast<num_t>(i * j); 
};

int main()
{

	auto && i = val{ 20, 0.5 };
	auto && j = val{ 50, 0.8 };

	auto && r = generate_error(f, i, j);

	std::cout << std::setprecision(18) << r << "\n";

	std::cout << std::setprecision(std::numeric_limits<num_t>::digits10 + 1) << r;

	std::cout << "\nŠú‘Ò‚³‚ê‚é’l : " << std::pow(20*20*0.8*0.8+50*50*0.5*0.5, 0.5);

}
