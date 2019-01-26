
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP
#define NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP

#include <nyaruga_util/make_compile_error.hpp>
#include <type_traits>

namespace nyaruga_util {

namespace nyaruga_util_impl {

template <size_t current_pos, size_t target_pos, typename Head,
          typename... Pack>
constexpr decltype(auto) pac_select_pos_obj_impl(Head && head,
                                                 Pack &&... pack) noexcept
{
   if constexpr (current_pos == target_pos)
      return head;
   else if constexpr (current_pos < target_pos)
      return pac_select_pos_obj_impl<
         current_pos + 1, target_pos>(std::forward<Pack>(pack)...);
   else
      NYARUGA_MAKE_COMPILE_ERROR("pos is out of pac");
}

} // namespace nyaruga_util_impl

// pac select pos expansion
// パラメーターパックの任意の位置のオブジェクトを返す関数
template <size_t pos, typename... Pack>
constexpr decltype(auto) pac_select_pos_obj(Pack &&... pack) noexcept
{
   return nyaruga_util_impl::pac_select_pos_obj_impl<1, pos>(
      std::forward<Pack>(pack)...);
}

/* 後で実装
template<size_t pos, typename ... Pack>
struct pac_select_pos_type(Pack && ... pack)
{
        using type =
        return nyaruga_util_impl::pac_select_pos_obj_impl<1,
pos>(std::forward<Pack>(pack)...);
}
*/

} // namespace nyaruga_util

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

#endif // #ifndef NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP