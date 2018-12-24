
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_NONMOVABLE_HPP
#define NYARUGA_UTIL_NONMOVABLE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace nyaruga_util {

class nonmovable
{
protected:
	constexpr nonmovable() noexcept = default;
	~nonmovable() = default;
	constexpr nonmovable(nonmovable const &) noexcept = default;
	constexpr nonmovable(nonmovable &&) noexcept = delete;
	nonmovable &
		operator=(nonmovable const &) noexcept = default;
	nonmovable &
		operator=(nonmovable &&) noexcept = delete;
};

} // namespace nyaruga_util

#endif // #ifndef NYARUGA_UTIL_NONMOVABLE_HPP