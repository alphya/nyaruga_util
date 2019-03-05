
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_BIND_SELECT_ARG_REPLACE_HPP
#define NYARUGA_UTIL_BIND_SELECT_ARG_REPLACE_HPP

#pragma once

#include <functional>
#include <nyaruga_util/make_compile_error.hpp>
#include <type_traits>

namespace nyaruga {

namespace util {

namespace impl {

template <size_t current_pos, size_t target_pos, typename F, typename Head,
          typename... Pack>
constexpr decltype(auto)
bind_select_arg_replace_impl(F && func, Head && head, Pack &&... pack) noexcept
{
   if constexpr (current_pos == target_pos)
      return std::bind(
         std::forward<F>(func), std::placeholders::_1,
         std::forward<Pack>(pack)...);
   else if constexpr (current_pos <
                      target_pos)
      return bind_select_arg_replace_impl<current_pos + 1,
                                          target_pos>(
         [func = std::forward<F>(func),
          head = std::forward<Head>(head)](auto &&... args) {
            return func(head, std::forward<decltype(args)>(args)...);
         },
         std::forward<Pack>(pack)...);
   else
      NYARUGA_MAKE_COMPILE_ERROR("pos is out of pac");
}

} // namespace impl

// �p�����[�^�[�p�b�N�̔C�ӂ̈ʒu�̈������p�����[�^�[�p�b�N�ɒu��������
// ���֐���Ԃ����K�֐�
// bind_other_than_any_pos_arg�ɂ悭���Ă���
template <size_t pos, typename F, typename... Pack>
constexpr decltype(auto) bind_select_arg_replace(F && func,
                                                 Pack &&... pack) noexcept
{
   return impl::bind_select_arg_replace_impl<1, pos>(
      std::forward<F>(func), std::forward<Pack>(pack)...);
}

} // namespace util

} // namespace nyaruga

/*

using namespace nyaruga::util;

#include <iostream>

int main()
{
        auto ll = [](auto && ... hh)
        {
                (std::cout << ... << std::forward<decltype(hh)>(hh));
        };

        auto fn = bind_select_arg_replace<2>(ll, 9, 10, 11);

        fn(22); // 9, 22, 11

}

*/

#endif // #ifndef NYARUGA_UTIL_BIND_SELECT_ARG_REPLACE_HPP