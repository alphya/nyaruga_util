//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CHAIN_BLOCK_HPP
#define NYARUGA_UTIL_CHAIN_BLOCK_HPP

#pragma once

#include <tuple>
#include <compare>
#include <utility>
#include <concepts>
#include <nyaruga_util/category.hpp>

// 引数を渡したり、受け取ったりできるブロックのような機能を持つモナド（あやしい）です
// 見ることができるローカル変数を制限したりしたいときに便利です

namespace nyaruga::util {

namespace chain_block_ {

inline struct exit_chain_t{} exit_chain;
inline struct begin_chain_t{} begin_chain;

template <typename ... Types>
struct pass_param : public std::tuple<Types...> { using std::tuple<Types...>::tuple; };

template <typename... Types>
pass_param(Types...)->pass_param<Types...>;

namespace detail
{
   template <typename T>
   constexpr auto is_pass_param_impl(T) -> std::false_type;

   template <template<class...> class T, typename ... Args>
      requires std::convertible_to<T<Args...>, pass_param<Args...>>
   constexpr auto is_pass_param_impl(T<Args...>) -> std::true_type;

   template <typename T>
   concept is_pass_param = decltype(is_pass_param_impl(std::declval<T>()))::value;

   template <typename T>
   concept is_begin_chain = std::convertible_to<T, begin_chain_t>;

   template <typename T>
   concept is_exit_chain = std::convertible_to<T, exit_chain_t>;
}

template <class T>
class chain_block {
private:
   T m_val;
   bool m_exit_flag;
public:
   constexpr chain_block() noexcept : m_exit_flag(false) {}
   constexpr chain_block(const exit_chain_t&) noexcept : m_exit_flag(true) {}
   constexpr chain_block(const T & x) noexcept : m_val(x), m_exit_flag(false) {}
   constexpr chain_block(T && x) noexcept : m_val(std::forward<T>(x)), m_exit_flag(false) {}
   template <std::convertible_to<T> U>
      requires std::is_copy_assignable_v<T>
   constexpr auto& operator = (const chain_block<U>& other) noexcept(std::is_nothrow_assignable_v<T>) { T tmp = other.m_val; swap(m_val, tmp); return *this; }
   template <std::convertible_to<T> U>
      requires std::is_move_assignable_v<T>
   constexpr auto& operator = (chain_block<U>&& other) noexcept(std::is_nothrow_move_assignable_v<T>) { m_val = std::move(other.m_val); return *this; }
   constexpr auto& unwrap() noexcept { return m_val; }
   constexpr auto exit_flag() { return m_exit_flag; }
   // constexpr auto operator <=> (const chain_block&) const noexcept = default;
};

template <class T>
   requires detail::is_exit_chain<T>
chain_block(T)->chain_block<std::nullptr_t>;

template <class T, class F>
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(f(x.unwrap())))
   -> chain_block<category::apply_mu<chain_block, decltype(f(x.unwrap()))>>
   requires category::kleisli_morphism_from<F, chain_block, T>
{
   return !x.exit_flag() ? f(x.unwrap()) : chain_block<category::apply_mu<chain_block, decltype(f(x.unwrap()))>>(exit_chain);
}

template <class T, class F>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(f(std::move(x.unwrap()))))
   -> chain_block<category::apply_mu<chain_block, decltype(f(std::move(x.unwrap())))>>
   requires category::kleisli_morphism_from<F, chain_block, T>
{
   return (!x.exit_flag()) ? f(std::move(x.unwrap())) : chain_block<category::apply_mu<chain_block, decltype(f(std::move(x.unwrap())))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_pass_param<T>
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(std::apply(f, x.unwrap())))
   -> chain_block<category::apply_mu<chain_block, decltype(std::apply(f, x.unwrap()))>>
{
   return !x.exit_flag() ? std::apply(f, x.unwrap()) : chain_block<category::apply_mu<chain_block, decltype(std::apply(f, x.unwrap()))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_pass_param<T>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(std::apply(f, std::move(x.unwrap()))))
   -> chain_block<category::apply_mu<chain_block, decltype(std::apply(f, std::move(x.unwrap())))>>
{
   return (!x.exit_flag()) ? std::apply(f, std::move(x.unwrap())) : chain_block<category::apply_mu<chain_block, decltype(std::apply(f, std::move(x.unwrap())))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_begin_chain<T> 
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(f()))
   -> chain_block<category::apply_mu<chain_block, decltype(f())>>
{
   return f();
}

template <class T, class F>
   requires detail::is_begin_chain<T>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(f()))
   -> chain_block<category::apply_mu<chain_block, decltype(f())>>
{
   return f();
}

template <class T>
constexpr auto ret(T&& x) noexcept -> decltype(chain_block(std::forward<T>(x)))
{
   return chain_block(std::forward<T>(x));
}

template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, chain_block>
constexpr auto operator|(const chain_block<T>& x, Func && f) noexcept(noexcept(f(x.unwrap())))
{
   return !x.exit_flag() ? chain_block<category::apply_morphism<T, Func>>{ f(x.unwrap()) } : chain_block<category::apply_morphism<T, Func>>{ exit_chain };
}
   
template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, chain_block>
constexpr auto operator|(chain_block<T>&& x, Func && f) noexcept(noexcept(f(std::move(x.unwrap()))))
{
   return !x.exit_flag() ? chain_block<category::apply_morphism<T, Func>>{ f(std::move(x.unwrap())) } : chain_block<category::apply_morphism<T, Func>>{ exit_chain };
}

template <typename T, typename Func>
   requires detail::is_pass_param<T> && category::kleisli_object<decltype(std::apply(std::declval<Func>(), std::declval<T>())), chain_block>
constexpr auto operator|(const chain_block<T>& x, Func && f) noexcept(noexcept(std::apply(f, x.unwrap())))
{
   return !x.exit_flag() ? chain_block<decltype(std::apply(f, x.unwrap()))>{ std::apply(f, x.unwrap()) } : chain_block<decltype(std::apply(f, x.unwrap()))>{ exit_chain };
}
   
template <typename T, typename Func>
   requires detail::is_pass_param<T> &&category::kleisli_object<decltype(std::apply(std::declval<Func>(), std::declval<T>())), chain_block>
constexpr auto operator|(chain_block<T>&& x, Func && f) noexcept(noexcept(std::apply(f, std::move(x.unwrap()))))
{
   return !x.exit_flag() ? chain_block<decltype(std::apply(f, std::move(x.unwrap())))>{ std::apply(f, std::move(x.unwrap())) }
                       : chain_block<decltype(std::apply(f, std::move(x.unwrap())))>{ exit_chain };
}

template <typename T, typename Func>
   requires detail::is_begin_chain<T> && category::kleisli_object<decltype(f()), chain_block> 
constexpr auto operator|(const chain_block<T> & x, Func && f) noexcept(noexcept(f()))
{
   return chain_block<decltype(f())>{ f() };
}

template <typename T, typename Func>
   requires detail::is_begin_chain<T> && category::kleisli_object<decltype(f()), chain_block> 
constexpr auto operator|(chain_block<T> && x, Func && f) noexcept(noexcept(f()))
{
   return chain_block<decltype(f())>{ f() };
}


} // namespace chain_block_

using namespace chain_block_;

} // nyaruga :: util

template <class... Types>
class std::tuple_size<nyaruga::util::pass_param<Types...>>
   : public std::integral_constant<std::size_t, sizeof...(Types)> {
};

#endif // NYARUGA_UTIL_CHAIN_BLOCK_HPP
