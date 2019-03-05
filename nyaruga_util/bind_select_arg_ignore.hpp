
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP
#define NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP

#pragma once

#include <functional>
#include <nyaruga_util/make_compile_error.hpp>
#include <type_traits>

namespace nyaruga {

namespace util {

namespace impl {

template <size_t current_pos, size_t target_pos, typename F, typename Head,
          typename... Pack>
constexpr decltype(auto) bind_select_arg_ignore_impl(F && func, Head && head,
                                                     Pack &&... pack) noexcept
{
   if constexpr (current_pos == target_pos)
      return std::bind(
         std::forward<F>(func), std::forward<Pack>(pack)...);
   else if constexpr (current_pos <
                      target_pos)
      return bind_select_arg_ignore_impl<current_pos + 1,
                                         target_pos>(
         [func = std::forward<F>(func), head = std::forward<Head>(head)](
            auto &&... args) { return func(head, args...); },
         std::forward<Pack>(pack)...);
   else
      NYARUGA_MAKE_COMPILE_ERROR("pos is out of pac");
}

} // namespace impl

// �p�����[�^�[�p�b�N�̔C�ӂ̈ʒu�̈����𖳎����āA
// ���̈ʒu�̌^�̈������Ƃ�֐���Ԃ����K�֐�
template <size_t pos, typename F, typename... Pack>
constexpr decltype(auto) bind_select_arg_ignore(F && func,
                                                Pack &&... pack) noexcept
{
   return impl::bind_select_arg_ignore_impl<1, pos>(
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

        // ���݁A�ϒ������֐��ɂ����Ή����ĂȂ��ł��@���@����ɂ͂����܂���
        // auto fn = [](int i, double d, float f, int h)
        // {
        //	   std::cout << i << " " << d << " " << f << " " << h;
        // };

        auto fn_ = bind_select_arg_ignore<3>(ll, 9, 10, 11);

        fn_(22); // 9, 11

}
*/

#endif // #ifndef NYARUGA_UTIL_BIND_SELECT_ARG_IGNORE_HPP