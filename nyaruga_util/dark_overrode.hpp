#pragma once

#include <type_traits>

namespace nyaruga_util {

	// operator >> のオーバーロード
	template<typename T, typename F>
	constexpr inline auto operator >> (T && arg, F && func) noexcept(noexcept(func(arg)))
		-> typename std::invoke_result_t<F, T>
	{
		return func(std::forward<T>(arg));
	}   

	// operator << のオーバーロード
	template<typename F, typename T>
	constexpr inline auto operator << (F && func, T && arg) noexcept(noexcept(func(arg)))
		-> typename std::invoke_result_t<F, T>
	{
		return func(std::forward<T>(arg));
	}

}
