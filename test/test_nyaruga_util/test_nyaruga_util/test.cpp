#include <iostream>

#include "../../../nyaruga_util/diff.hpp"



#include <functional>
#include <type_traits>
#include "../../../nyaruga_util/bind_other_than_any_pos_arg.hpp"

int main()
{
	

	auto fn = [](int i, double d, float f, int h)
	{
		std::cout << i << " " << d << " " << f << " " << h;
	};


	// fn(66.f, <placeholder>, 89.f, 47)
	//    1     2              3     4   <- template arg
	//          ª
	auto && aaa = nyaruga_util::bind_other_than_any_pos_arg<2>(fn, 66.f, 89.f, 47);

	// 66 999 89 47
	aaa(999.f);
	
}
