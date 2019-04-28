
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_POINTER_WRAPPER_HPP
#define NYARUGA_UTIL_POINTER_WRAPPER_HPP

#pragma once

#include <type_traits>
#include <memory>

namespace nyaruga {

namespace util {

namespace pointer_wrapper_ { // protection from unintended ADL

// ポインターをラップします
// delete したくないポインターに使えます
template <typename T>
class pointer_wrapper final {
public:
   using pointer = std::add_pointer_t<std::remove_reference_t<T>>;
public:
   constexpr pointer_wrapper() noexcept = default;

   pointer_wrapper(const pointer_wrapper & other) noexcept
   {
      this->m_ptr = other.get();
   }

   template <class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
   pointer_wrapper(const pointer_wrapper<T2> & other) noexcept
   {
      this->m_ptr = other.get();
   }

   pointer_wrapper(pointer_wrapper && other) noexcept
   {
      this->m_ptr = other.get();
      other.reset();
   }

   template <class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
   pointer_wrapper(pointer_wrapper<T2> && other) noexcept
   {
      this->m_ptr = other.get();
      other.reset();
   }

   pointer_wrapper(pointer& other) noexcept
   {
      this->m_ptr = other;
   }

   pointer_wrapper(pointer&& other) noexcept
   {
      this->m_ptr = std::move(other);
      other = nullptr;
   }

   template <class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
   pointer_wrapper(std::add_pointer_t<std::remove_reference_t<T2>>& other) noexcept
   {
      this->m_ptr = other;
   }

   template <class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
   pointer_wrapper(std::add_pointer_t<std::remove_reference_t<T2>>&& other) noexcept
   {
      this->m_ptr = std::move(other);
      other = nullptr;
   }

   ~pointer_wrapper() noexcept = default;

   pointer_wrapper & operator=(const pointer_wrapper& right) noexcept
   {
      pointer_wrapper(right).swap(*this);
      return *this;
   }

   template <class T2>
   pointer_wrapper & operator=(const pointer_wrapper<T2>& right) noexcept
   {
      pointer_wrapper(right).swap(*this);
      return *this;
   }

   pointer_wrapper & operator=(pointer_wrapper && right) noexcept
   {
      pointer_wrapper(std::move(right)).swap(*this);
      return *this;
   }

   template <class T2>
   pointer_wrapper & operator=(pointer_wrapper<T2> && right) noexcept
   {
      pointer_wrapper(std::move(right)).swap(*this);
      return *this;
   }

   template <class T2>
   pointer_wrapper & operator=(const std::add_pointer_t<std::remove_reference_t<T2>>& right) noexcept
   {
      pointer_wrapper(right).swap(*this);
      return *this;
   }

   template <class T2>
   pointer_wrapper & operator=(const std::add_pointer_t<std::remove_reference_t<T2>>&& right) noexcept
   {
      pointer_wrapper(std::move(right)).swap(*this);
      right = nullptr;
      return *this;
   }

   [[nodiscard]] pointer get() const noexcept
   {
      return this->m_ptr;
   }

   [[nodiscard]] std::add_lvalue_reference_t<std::remove_reference_t<T>> operator*() const
   {
      return *get();
   }

   [[nodiscard]] pointer operator->() const noexcept
   {
      return this->m_ptr();
   }

   explicit operator bool() const noexcept
   {
      return get() != nullptr;
   }

   pointer release() noexcept
   {
      pointer ans = get();
      this->m_ptr = nullptr;
      return ans;
   }

   void reset(pointer p = nullptr) noexcept
   {
      pointer_wrapper(p).swap(*this);
   }

   void swap(pointer_wrapper& other) noexcept
   {
      std::swap(this->m_ptr, other.m_ptr);
   }

   template <class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
   void swap(pointer_wrapper<T2> & other) noexcept
   {
      std::swap(this->m_ptr, other.m_ptr);
   }

private:
   pointer m_ptr = nullptr;
};

template <class T>
pointer_wrapper(std::add_pointer_t<std::remove_reference_t<T>>) -> pointer_wrapper<T>;

template <class T, class T2, std::enable_if_t<std::is_convertible<T2, T>::value, int> = 0>
void swap(pointer_wrapper<T> & left, pointer_wrapper<T2> & right) noexcept
{
   left.swap(right);
}

} // namespace pointer_wrapper_

using namespace pointer_wrapper_;

} // namespace util

} // namespace nyaruga

#endif // #ifndef NYARUGA_UTIL_POINTER_WRAPPER_HPP