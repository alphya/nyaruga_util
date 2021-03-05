/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_CONFIG_HPP
#define NYARUGA_UTIL_CONFIG_HPP

#include <boost/config.hpp>

// -------------------------
// User-adjustable macros
// -------------------------

// If you have not https://github.com/tcbrindle/rational.git installed, comment out here.
#define NYARUGA_UTIL_HAS_TCBRINDLER_RATIONAL

// If you have not EIGEN installed, comment out here.
#define NYARUGA_UTIL_HAS_EIGEN

#if defined(NYARUGA_UTIL_DOXYGEN_INVOKED) || \
   (defined(NDEBUG) && !defined(NYARUGA_UTIL_CONFIG_DISABLE_ASSERTIONS))

//! Disables assertions.
#   define NYARUGA_UTIL_CONFIG_DISABLE_ASSERTIONS

#endif

#endif // #ifndef NYARUGA_UTIL_CONFIG_HPP
