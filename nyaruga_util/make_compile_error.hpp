
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP
#define NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP

#include <boost/static_assert.hpp>
#include <boost/preprocessor/stringize.hpp>

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#define NYARUGA_MAKE_COMPILE_ERROR( ... ) \
static_assert(std::bool_constant<false>::value, \
"\n\n\nNYARUGA_UTIL MAKED COMPILE ERROR!\n" \
"file: " BOOST_PP_STRINGIZE(__FILE__) "\n" \
"line: " BOOST_PP_STRINGIZE(__LINE__) "\n" \
"message: " BOOST_PP_STRINGIZE(__VA_ARGS__) "\n\n" \
);

#endif // #ifndef NYARUGA_UTIL_MAKE_COMPILE_ERROR_HPP

/*
使い道：
　実体化させた場合、コンパイルエラーになります。
　メッセージ付きコンパイルエラーにどうぞ
*/