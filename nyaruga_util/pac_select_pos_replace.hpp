#pragma once


#include "false_v.hpp"
#include <functional>

namespace nyaruga_util {

	namespace nyaruga_util_impl {

		template<size_t current_pos, size_t target_pos, typename F, typename Head, typename ... Pack>
		constexpr decltype(auto)
			pac_select_pos_replace_impl(F && func, Head && head, Pack && ... pack) noexcept
		{
			if constexpr (current_pos == target_pos)
				return std::bind(std::forward<F>(func), std::placeholders::_1, std::forward<Pack>(pack)...);

			else if constexpr (current_pos < target_pos)

				return pac_select_pos_replace_impl<current_pos + 1, target_pos>(
					[func = std::forward<F>(func), head = std::forward<Head>(head)]
			(auto && ... args) {
				return func(head, std::forward<decltype(args)>(args)...);
			},
				std::forward<Pack>(pack)...
				);

			else
				static_assert(false_v<Head>, "pos is out of pac");
		}

	}  // nyaruga_util_impl

	// パラメーターパックの任意の位置の引数をパラメーターパックに置き換える関数
	// bind_other_than_any_pos_argによく似ている
	template<size_t pos, typename F, typename ... Pack>
	constexpr decltype(auto) pac_select_pos_replace(F && func, Pack && ... pack) noexcept
	{
		return nyaruga_util_impl::pac_select_pos_replace_impl<1, pos>(
			std::forward<F>(func), std::forward<Pack>(pack)...);
	}


}  // nyaruga_util

/*

int main()
{
	auto ll = [](auto && ... hh)
	{
		(std::cout << ... << std::forward<decltype(hh)>(hh));
	};

	auto fn = pac_select_pos_replace<2>(ll, 9, 10, 11);

	fn(22); // 9, 22, 11

}
*/