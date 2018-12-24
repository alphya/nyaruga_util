
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_MEM_FUN_WRAP_HPP
#define NYARUGA_UTIL_MEM_FUN_WRAP_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <type_traits>

namespace nyaruga_util {

template
<
	typename Ret,
	typename C,
	typename Obj,
	typename ... Args
>
auto mem_fun_wrap(Ret(C::* && f)(Args...), Obj * && obj)
{
	return
		[
			f = std::forward<decltype(f)>(f),
			obj = std::forward<decltype(obj)>(obj)
		]
	(auto && ... args) mutable
	{
		return (obj->*f)(std::forward<decltype(args)>(args)...);
	};
}

template
<
	typename Ret,
	typename C,
	typename Obj,
	typename ... Args
>
auto mem_fun_wrap(Ret(C::* && f)(Args...), Obj && obj)
{
	return
		[
			f = std::forward<decltype(f)>(f),
			obj = std::forward<decltype(obj)>(obj)
		]
	(auto && ... args) mutable
	{
		return (obj.*f)(std::forward<decltype(args)>(args)...);
	};
}

} // namespace nyaruga_util

#endif // #ifndef NYARUGA_UTIL_MEM_FUN_WRAP_HPP