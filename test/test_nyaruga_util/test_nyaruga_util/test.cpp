
#include <iostream>

#include "../../../nyaruga_util/value_including_error.hpp"

auto main() -> int
{
	// 誤差を含む値の定義 { 値, 誤差 }
	auto && x = nyaruga_util::value_including_error{ 20, 0.5 };
	auto && y = nyaruga_util::value_including_error{ 50, 0.8 };

	// 誤差を含む値の関数定義　これは単純な掛け算
	decltype(auto) f = [](nyaruga_util::num_t x, nyaruga_util::num_t y) {
		return static_cast<nyaruga_util::num_t>(x * y);
	};

	// 普通に関数を実行した値
	auto && v = f(x, y);

	// 誤差を計算
	auto && e = nyaruga_util::generate_error(f, x, y);

	// だいたいあってる程度の値です
	// 単純な掛け算で、有効数字17桁くらいでした
	std::cout << std::setprecision(18) 
		<< "値は：" << v << "\n誤差は：" << e << "\n";

}
