
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_WEAK_PTR_FOR_UNIQUE_HPP
#define NYARUGA_UTIL_WEAK_PTR_FOR_UNIQUE_HPP

#pragma once

#include <type_traits>
#include <memory>

namespace nyaruga {

namespace util {

namespace weak_ptr_for_unique_ {  // protection from unintended ADL

// std::unique_ptr 版 の weak_ptr です
// unique_ptr からポインタを get() してきたいけど、生のポインタは
// 使いたくないな...って時に使えます
template <typename T>
class weak_ptr_for_unique final {
public:
   constexpr weak_ptr_for_unique() noexcept = default;

   weak_ptr_for_unique(const weak_ptr_for_unique & other) noexcept
   {
      this->m_ptr = other.get();
   }

   template <class T2, std::enable_t<std::is_convertible<T2, T>::value, int> = 0>
   weak_ptr_for_unique(const std::unique_ptr<T2> & other) noexcept
   {
      this->m_ptr = other.get();
   }

   template <class T2, std::enable_t<std::is_convertible<T2, T>::value, int> = 0>
   weak_ptr_for_unique(const weak_ptr_for_unique<T2> & other) noexcept
   {
      this->m_ptr = other.get();
   }

   weak_ptr_for_unique(weak_ptr_for_unique && other) noexcept
   {
      this->m_ptr = other.get();
      other.reset();
   }

   template <class T2, std::enable_t<std::is_convertible<T2, T>::value, int> = 0>
   weak_ptr_for_unique(weak_ptr_for_unique<T2> && other) noexcept
   {
      this->m_ptr = other.get();
      other.reset();
   }

   ~weak_ptr_for_unique() noexcept = default;

   weak_ptr_for_unique & operator=(const weak_ptr_for_unique & right) noexcept
   {
      weak_ptr_for_unique(right).swap(*this);
      return *this;
   }

   template <class T2>
   weak_ptr_for_unique & operator=(const weak_ptr_for_unique<T2> & right) noexcept
   {
      weak_ptr_for_unique(right).swap(*this);
      return *this;
   }

   template <class T2>
   weak_ptr_for_unique & operator=(const std::unique_ptr<T2> & right) noexcept
   {
      weak_ptr_for_unique(right).swap(*this);
      return *this;
   }

   weak_ptr_for_unique & operator=(weak_ptr_for_unique && right) noexcept
   {
      weak_ptr_for_unique(std::move(right)).swap(*this);
      return *this;
   }

   template <class T2>
   weak_ptr_for_unique & operator=(weak_ptr_for_unique<T2> && right) noexcept
   {
      weak_ptr_for_unique(std::move(right)).swap(*this);
      return *this;
   }

   [[nodiscard]] std::add_pointer_t<std::remove_reference_t<T>> get() const noexcept
   {
      return this->m_ptr;
   }

   [[nodiscard]] std::add_lvalue_reference_t<T> operator*() const
   {
      return *get();
   }

   [[nodiscard]] std::add_pointer_t<std::remove_reference_t<T>> operator->() const noexcept
   {
      return this->m_ptr();
   }

   explicit operator bool() const noexcept
   {
      return get() != pointer();
   }

   std::add_pointer_t<std::remove_reference_t<T>> release() noexcept
   {
      std::add_pointer_t<std::remove_reference_t<T>> ans = get();
      this->m_ptr = nullptr;
      return ans;
   }

   void reset() noexcept
   {
      weak_ptr_for_unique().swap(*this);
   }

   template <class T2, std::enable_t<std::is_convertible<T2, T>::value, int> = 0>
   void swap(weak_ptr_for_unique<T2> & other) noexcept
   {
      std::swap(this->ptr, other.ptr);
   }

private:
   std::add_pointer_t<std::remove_reference_t<T>> m_ptr = nullptr;
};

template <class T>
weak_ptr_for_unique(std::unique_ptr<T>)->weak_ptr_for_unique<T>;

} // namespace weak_ptr_for_unique_

template<typename T>
using weak_ptr_for_unique = weak_ptr_for_unique_::weak_ptr_for_unique<T>;

template <class T>
void swap(weak_ptr_for_unique<T> & left, weak_ptr_for_unique<T> & right) noexcept
{ // swap contents of _Left and _Right
   left.swap(right);
}

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_WEAK_PTR_FOR_UNIQUE_HPP