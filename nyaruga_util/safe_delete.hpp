
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_SAFE_DELETE_HPP
#define NYARUGA_UTIL_SAFE_DELETE_HPP

#pragma once

namespace nyaruga {

namespace util {

template <class T>
inline constexpr void safe_delete(T *& p) noexcept
{
   //  不完全な型のポインタをdeleteしようとした時にコンパイルエラーにする
   typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
   (void)sizeof(type_must_be_complete);
   if (p)
      delete p;
   p = nullptr;
}

template <class T>
inline constexpr void safe_delete_array(T *& p) noexcept
{
   typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
   (void)sizeof(type_must_be_complete);
   if (p)
      delete[] p;
   p = nullptr;
}

template <class T>
inline constexpr void safe_Release(T *& p) noexcept
{
   typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
   (void)sizeof(type_must_be_complete);
   if (p)
      p->Release();
   // p = nullptr;
}

template <typename T = void>
inline constexpr void safe_delete(...) noexcept
{
   static_assert([] { return false; }(), "This arg can not delete.");
}

template <typename T = void>
inline constexpr void safe_delete_array(...) noexcept
{
   static_assert([] { return false; }(), "This arg can not delete.");
}

template <typename T = void>
inline constexpr void safe_Release(...) noexcept
{
   static_assert([] { return false; }(), "This arg has not Release.");
}

} // namespace util

} // namespace nyaruga

/* 使い方
コピペして任意のメンバ関数を呼びたいときとかに使う。適宜改造して

*/

/* 使い時
型安全な解放とか

*/

#endif // #ifndef NYARUGA_UTIL_SAFE_DELETE_HPP