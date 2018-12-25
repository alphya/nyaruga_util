
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP
#define NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <Windows.h>
#include <string>

namespace nyaruga_util {

void print_dbg(std::string msg)
{
	char buf[128];
	wsprintf(buf, msg.c_str());
	OutputDebugString(buf);
}

} // namespace nyaruga_util

#endif // #ifndef NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP