
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_DIFF_HPP
#define NYARUGA_UTIL_DIFF_HPP

#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <cmath>
#include <limits>
#include <nyaruga_util/get_arg_type.hpp>
#include <type_traits>

namespace nyaruga_util {

// ������1,000�r�b�g�������������_���̒�`�B���Ȃ肷����
using num_t =
   boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000>>;

// �������܂�
// 7�_�ߎ� 5�_��萸�x����
template <typename F, typename Arg>
decltype(auto) diff(F && f, Arg && x) noexcept(noexcept(f(x)))
{
   num_t && h = DBL_EPSILON;
   num_t && y1 = f(static_cast<num_t>(x + h));
   num_t && y2 = f(static_cast<num_t>(x - h));
   num_t && y3 = f(static_cast<num_t>(x + 2 * h));
   num_t && y4 = f(static_cast<num_t>(x - 2 * h));
   num_t && y5 = f(static_cast<num_t>(x + 3 * h));
   num_t && y6 = f(static_cast<num_t>(x - 3 * h));

   return static_cast<num_t>(
      (std::forward<num_t>(y5) - 9 * std::forward<num_t>(y3) +
       45 * std::forward<num_t>(y1) - 45 * std::forward<num_t>(y2) +
       9 * std::forward<num_t>(y4) - std::forward<num_t>(y6)) /
      (60 * std::forward<num_t>(h)));
}

// 5�_�ߎ�
template <typename F, typename Arg>
decltype(auto) diff_fast(F && f, Arg && x) noexcept(noexcept(f(x)))
{
   auto && h = DBL_EPSILON;
   auto && y1 = f(x + h);
   auto && y2 = f(x - h);
   auto && y3 = f(x + 2 * h);
   auto && y4 = f(x - 2 * h);
   return (y4 - 8 * y2 + 8 * y1 - y3) / (12 * h);
}

} // namespace nyaruga_util

/*

using namespace nyaruga_util;

// ��������֐���`�B
// ���x�̂��߁Adouble�Ȃǂł͂Ȃ�nyaruga_util::num_t���g���Ă�������
num_t fn(num_t && x)
{
        return (x*x*x);
}

int main()
{

        num_t d = diff(fn, 3.f);

        std::cout << std::setprecision(std::numeric_limits<num_t>::digits10 + 1)
                << "���҂����l�F27" << "\ndiff : " << d << "\ndelta: " <<
DBL_EPSILON;

}

*/

#endif // #ifndef NYARUGA_UTIL_DIFF_HPP