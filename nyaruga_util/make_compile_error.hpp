
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP
#define NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP

#pragma once

#include <boost/preprocessor/stringize.hpp>

#define NYARUGA_MAKE_COMPILE_ERROR(...) \
   static_assert([] { return false; }(), \
                 "\n\n\nMAKED COMPILE ERROR!\n" \
                 "file: " BOOST_PP_STRINGIZE(__FILE__) "\n" \
                                                       "line: " BOOST_PP_STRINGIZE(__LINE__) "\n" \
                                                                                             "message: " BOOST_PP_STRINGIZE(#__VA_ARGS__) "\n\n")

/*
�g�����F
�@���̉��������ꍇ�A�R���p�C���G���[�ɂȂ�܂��B
�@���b�Z�[�W�t���R���p�C���G���[�ɂǂ���
*/

#endif // #ifndef NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP
