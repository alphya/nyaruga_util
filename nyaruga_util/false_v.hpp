#pragma once

namespace nyaruga_util {
	// static_assert �̕]�����A�e���v���[�g��
	// �C���X�^���X���܂Œx�点�邽�߂Ɏg��
	template <class T>
	constexpr bool false_v = false;
}

/*

// this code is not compile error.

template <class T>
void test()
{
	using namespace nyaruga_util;
	if constexpr (true)
	{
		int a = 10;
	}
	else
	{
		static_assert(false_v<T>, "nyannyan");
	}
}


int main()
{
	using namespace nyaruga_util;

	test<int>();

}

*/