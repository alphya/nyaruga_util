#pragma once

#include <type_traits>

namespace nyaruga_util
{
	// operator >> のオーバーロード()
	template<typename T, typename F>
	static inline auto operator >> (T && arg, F && func) -> typename std::invoke_result_t<F, T>
	{
		return func(std::forward<decltype(arg)>(arg));
	}   
}


/* 使い方

// 任意の関数オブジェクトつくる。引数は一つ
struct windows_expect
{
	windows_expect() noexcept : errorCode(-1) {}; // 特に何も指定しなければ-1を返す
	windows_expect(int code) noexcept : errorCode(code) {};
	inline void operator () (HRESULT && arg)
	{
		return SUCCEEDED(std::move(arg)) ? void(0) : ::exit(errorCode);
	}
private: int errorCode;
};

// アダプタとして渡す
CoInitialize(nullptr) // これは普通の関数。
    >> windows_expect(0); // 上の関数の戻り値を引数に取ってる。
// こんな感じで、どんどんつなげられる。

*/

/* 使い時
エラー処理を簡単に書いたりとか
お決まりのパターンを簡単に書いたりとか
関数型風プログラミングとか

*/
