
/*!
 * @file 
 * @brief 
 * @copyright alphya 2019-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_RAII_HPP
#define NYARUGA_UTIL_RAII_HPP

#pragma once

#include <boost/core/noncopyable.hpp>
#include <utility>
#include <type_traits>
#include <functional>

namespace nyaruga {

namespace util {

   namespace detail {

      template <class, class, class = void>
      struct is_resource_derletable : std::false_type {
      };

      template <class Deleter, class T>
      struct is_resource_derletable<Deleter, T, std::void_t<
         decltype(std::declval<Deleter>()(std::declval<T>()))>
      > : std::true_type {
      };

      template <class Deleter, class T>
      concept resource_deletable = is_resource_derletable<Deleter, T>::value;

   }

   template <typename Deleter, typename T>
      requires detail::resource_deletable<Deleter, T>
   class raii : boost::noncopyable
   {
   private:
      T&& m_resource_;
   public:
      // please use std::move
      raii(T&& resource) : m_resource_(std::move(resource)) {};
      raii(T&& resource, Deleter) : m_resource_(std::move(resource)) {};
      ~raii() { Deleter{}(std::forward<decltype(m_resource_)>(m_resource_)); }

      T& get() { return m_resource_; }
   };

} // namespace nyaruga::util

#endif // #ifndef NYARUGA_UTIL_RAII_HPP
