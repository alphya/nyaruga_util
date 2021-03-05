
/*!
 * @file 
 * @brief 
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef NYARUGA_UTIL_NONMOVABLE_HPP
#define NYARUGA_UTIL_NONMOVABLE_HPP

#pragma once

namespace nyaruga {

namespace util {

namespace nonmovable_ { // protection from unintended ADL

class nonmovable {
protected:
   constexpr nonmovable() noexcept = default;
   ~nonmovable() = default;
   constexpr nonmovable(nonmovable const &) noexcept = default;
   constexpr nonmovable(nonmovable &&) noexcept = delete;
   nonmovable & operator=(nonmovable const &) noexcept = default;
   nonmovable & operator=(nonmovable &&) noexcept = delete;
};
} // namespace nonmovable_

using nonmovable = nonmovable_::nonmovable;

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_NONMOVABLE_HPP