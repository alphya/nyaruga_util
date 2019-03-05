
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP
#define NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP

#pragma once

#include <nyaruga_util/make_compile_error.hpp>
#include <type_traits>

namespace nyaruga {

namespace util {

namespace impl {

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

} // namespace impl

// pac select pos expansion
// �p�����[�^�[�p�b�N�̔C�ӂ̈ʒu�̃I�u�W�F�N�g��Ԃ��֐�
template <size_t pos, typename... Pack>
constexpr decltype(auto) pac_select_pos_obj(Pack &&... pack) noexcept
{
   return nyaruga::util::impl::pac_select_pos_obj_impl<1, pos>(
      std::forward<Pack>(pack)...);
}

/* ��Ŏ���
template<size_t pos, typename ... Pack>
struct pac_select_pos_type(Pack && ... pack)
{
        using type =
        return impl::pac_select_pos_obj_impl<1,
pos>(std::forward<Pack>(pack)...);
}
*/

} // namespace util

} // namespace nyaruga

/*

#include <iostream>

int main()
{
        auto ll = [](auto && ... hh)
        {
                std::cout << nyaruga::util::pac_select_pos_obj<2>(hh...);
        };

        ll(9, 10, 11); // 10

}
*/

#endif // #ifndef NYARUGA_UTIL_PAC_SELECT_POS_OBJ_HPP