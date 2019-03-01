
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP
#define NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP

#pragma once

#include <nyaruga_util/bind_select_arg_replace.hpp>
#include <nyaruga_util/diff.hpp>
#include <nyaruga_util/get_arg_num.hpp>
#include <nyaruga_util/pac_select_pos_obj.hpp>

namespace nyaruga {

namespace util {

namespace value_including_error_ { // protection from unintended ADL

// �덷���܂ޒl
struct value_including_error {
   num_t value; // �l
   num_t error; // �덷
   operator num_t() { return value; }
   operator num_t() const { return value; }
};
} // namespace value_including_error_

using value_including_error = value_including_error_::value_including_error;

namespace nyaruga_util_impl {

// �Δ���^2 * �덷^2 ���A��ϐ��������v�Z���āA�ċA
template <std::size_t count, std::size_t max, typename F, typename... Args>
decltype(auto) generate_error_impl(F && f, Args &&... args)
{
   if constexpr (count > max)
      return num_t{ 0 };
   else {
      auto && current_val_obj = pac_select_pos_obj<count>(args...);
      auto && current_val_error = current_val_obj.error;
      auto && partial_diff =
         diff(bind_select_arg_replace<count>(f, args...), current_val_obj.value);
      auto && result = boost::multiprecision::pow(partial_diff, 2) *
                       boost::multiprecision::pow(current_val_error, 2);
      return static_cast<num_t>(
         result + generate_error_impl<count + 1, max>(
                     std::forward<F>(f), std::forward<Args>(args)...));
   }
}

} // namespace nyaruga_util_impl

// �덷�̓`�d�����A�v�Z�B�덷��Ԃ�
// Args �ɂ́Avalue_including_error���g�p���Ă�������
template <typename F, typename... Args>
decltype(auto) generate_error(F && f, Args &&... args)
{
   return static_cast<num_t>(boost::multiprecision::pow(
      nyaruga_util_impl::generate_error_impl<1, sizeof...(args)>(
         std::forward<F>(f), std::forward<decltype(args)>(args)...),
      0.5l));
}

} // namespace util

} // namespace nyaruga

/*

#include "value_including_error.hpp"
#include <iostream>

auto main() -> int
{
        // �덷���܂ޒl�̒�` { �l, �덷 }
        auto && x = nyaruga_util::value_including_error{ 20, 0.5 };
        auto && y = nyaruga_util::value_including_error{ 50, 0.8 };

        // �덷���܂ޒl�̊֐���`�@����͒P���Ȋ|���Z
        decltype(auto) f = [](nyaruga_util::num_t x, nyaruga_util::num_t y) {
                return static_cast<nyaruga_util::num_t>(x * y);
        };

        // ���ʂɊ֐������s�����l
        auto && v = f(x, y);

        // �덷���v�Z
        auto && e = nyaruga_util::generate_error(f, x, y);

        // �������������Ă���x�̒l�ł�
        // �P���Ȋ|���Z�ŁA�L������17�����炢�ł���
        std::cout << std::setprecision(18)
                << "�l�́F" << v << "\n�덷�́F" << e << "\n";

}


*/

#endif // #ifndef NYARUGA_UTIL_VALUE_INCLUDING_ERROR_HPP
