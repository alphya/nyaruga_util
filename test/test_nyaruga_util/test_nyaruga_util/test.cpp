
#include <iostream>

#include "../../../nyaruga_util/value_including_error.hpp"

auto main() -> int
{
	// �덷���܂ޒl�̒�` { �l, �덷 }
	auto && x = nyaruga_util::value_including_error{ 20, 0.5 };
	auto && y = nyaruga_util::value_including_error{ 50, 0.8 };

	// �덷���܂ޒl�̊֐���`�@����͒P���Ȋ|���Z
	decltype(auto) f = [](nyaruga_util::num_t x, nyaruga_util::num_t y) {
		return static_cast<nyaruga_util::num_t>(x * y);
	};

	// ���ʂɊ֐������s�����l
	auto && v = f(x, y);

	// �덷���v�Z
	auto && e = nyaruga_util::generate_error(f, x, y);

	// �������������Ă���x�̒l�ł�
	// �P���Ȋ|���Z�ŁA�L������17�����炢�ł���
	std::cout << std::setprecision(18) 
		<< "�l�́F" << v << "\n�덷�́F" << e << "\n";

}
