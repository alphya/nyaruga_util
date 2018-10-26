#pragma	once

#include "false_v.hpp"

namespace nyaruga_util
{

	template <class T>
	inline void safe_delete(T*& p) {
		//  不完全な型のポインタをdeleteしようとした時にコンパイルエラーにする
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		if (p) delete p;
		p = nullptr;
	}
    
	template <class T>
	inline void safe_delete_array(T*& p) {
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		if (p) delete[] p;
		p = nullptr;
	}

	template<typename T = void>
	inline constexpr void safe_delete(...) noexcept
	{
		static_assert(false, "This arg can not delete.");
	}

	template<typename T = void>
	inline constexpr void safe_delete_array(...) noexcept
	{
		static_assert(false, "This arg can not delete.");
	}

}

/* 使い方
コピペして任意のメンバ関数を呼びたいときとかに使う。適宜改造して

*/

/* 使い時
型安全な解放とか

*/