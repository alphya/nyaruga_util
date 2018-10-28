#pragma once


#include "false_v.hpp"

namespace nyaruga_util {

	namespace nyaruga_util_impl {

		template<size_t current_pos, size_t target_pos, typename Head, typename ... Pack>
		constexpr decltype(auto) pac_select_pos_obj_impl(Head && head, Pack && ... pack) noexcept
		{
			if constexpr (current_pos == target_pos)
				return head;
			else if constexpr (current_pos < target_pos)
				return pac_select_pos_obj_impl<current_pos + 1, target_pos>(std::forward<Pack>(pack)...);
			else
				static_assert(false_v<Head>, "pos is out of pac");
		}

	}  // nyaruga_util_impl

	// pac select pos expansion
	// パラメーターパックの任意の位置のオブジェクトを返す関数
	template<size_t pos, typename ... Pack>
	constexpr decltype(auto) pac_select_pos_obj(Pack && ... pack) noexcept
	{
		return nyaruga_util_impl::pac_select_pos_obj_impl<1, pos>(std::forward<Pack>(pack)...);
	}

	/* 後で実装
	template<size_t pos, typename ... Pack>
	struct pac_select_pos_type(Pack && ... pack)
	{
		using type =
		return nyaruga_util_impl::pac_select_pos_obj_impl<1, pos>(std::forward<Pack>(pack)...);
	}
	*/

}  // nyaruga_util

/*

#include <iostream>

int main()
{
	auto ll = [](auto && ... hh)
	{
		std::cout << nyaruga_util::pac_select_pos_obj<2>(hh...);
	};

	ll(9, 10, 11); // 10

}
*/