
namespace
{
	// 安全な Release (Relese()メンバ関数が存在しない場合コンパイルエラー)
	template<typename T>
	static inline constexpr auto safeRelease(T && ptr) noexcept -> decltype(ptr->Release(), std::void_t<T>())
	{
		if (ptr) ptr->Release();
	}
    
	template<typename ... T>
	static inline constexpr auto safeRelease(...) noexcept -> void
	{
		static_assert(false, "This arg can not call Release()");
	}
}

/* 使い方
コピペして任意のメンバ関数を呼びたいときとかに使う。適宜改造して

*/

/* 使い時
型安全な解放とか

*/