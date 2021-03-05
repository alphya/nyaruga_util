
/*!
 * @file 
 * @brief 
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP
#define NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP

#pragma once

#include <functional>
#include <nyaruga_util/make_compile_error.hpp>
#include <type_traits>

namespace nyaruga {

namespace util {

namespace impl {

template <std::size_t current_pos, std::size_t arg_pos, typename F>
constexpr decltype(auto) bind_other_than_select_arg_impl(F && f) noexcept
{
   return [](auto && f) {
      return std::bind(std::forward<decltype(f)>(f), std::placeholders::_1);
   }(std::forward<decltype(f)>(f));
}

template <std::size_t current_pos, std::size_t arg_pos, typename F, typename H,
          typename... Arg>
constexpr decltype(auto)
bind_other_than_select_arg_impl(F && f, H && head, Arg &&... arg) noexcept
{
   if constexpr (current_pos < arg_pos)
      return bind_other_than_select_arg_impl<current_pos + 1, arg_pos>(
         [f = std::forward<decltype(f)>(f),
          head = std::forward<decltype(head)>(head)](auto &&... args) {
            return f(head, std::forward<decltype(args)>(args)...);
         },
         std::forward<decltype(arg)>(arg)...);
   else if constexpr (current_pos == arg_pos)
      return
         [](auto && f, auto && head, auto &&... args) {
            return std::bind(std::forward<decltype(f)>(f), std::placeholders::_1,
                             std::forward<decltype(head)>(head),
                             std::forward<decltype(args)>(args)...);
         }(std::forward<decltype(f)>(f), std::forward<decltype(head)>(head),
           std::forward<decltype(arg)>(arg)...);
   else
      NYARUGA_MAKE_COMPILE_ERROR("This function does not has requests argument.");
}

} // namespace impl

// �w�肵�������ȊO��K�p�����֐���Ԃ����K�֐� f(arg_pos = 1, 2, 3, 4...)
template <std::size_t arg_pos, typename F, typename... Arg>
constexpr decltype(auto)
bind_other_than_select_arg(F && f, Arg &&... arg) noexcept
{
   return impl::bind_other_than_select_arg_impl<1, arg_pos>(
      std::forward<F>(f), std::forward<Arg>(arg)...);
}

} // namespace util

} // namespace nyaruga

/*


#include <iostream>

int main()
{

        auto fn = [](int i, double d, float f, int h)
        {
                std::cout << i << " " << d << " " << f << " " << h;
        };

        // fn(66.f, <placeholder>, 89.f, 47)
        //    1     2              3     4   <- template arg
        //          ��
        auto && aaa = nyaruga::util::bind_other_than_select_arg<2>(fn, 66.f,
89.f, 47);

        // 66 999 89 47
        aaa(999.f);

}
*/

#endif // #ifndef NYARUGA_UTIL_BIND_OTHER_THAN_SELECT_ARG_HPP