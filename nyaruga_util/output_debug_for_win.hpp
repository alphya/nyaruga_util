
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP
#define NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP

#pragma once

#include <Windows.h>
#include <string>

namespace nyaruga {

namespace util {

void print_dbg(std::string msg)
{
#ifdef _DEBUG
   char buf[128];
   wsprintf(buf, msg.c_str());
   OutputDebugString(buf);
#endif
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_OUTPUT_DEBUG_FOR_WIN_HPP