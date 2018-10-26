#pragma once

#include <type_traits>

namespace nyaruga_util
{
	// operator >> のオーバーロード
	template<typename T, typename F>
	static inline auto operator >> (T && arg, F && func) -> typename std::invoke_result_t<F, T>
	{
		return func(std::forward<decltype(arg)>(arg));
	}   
}
