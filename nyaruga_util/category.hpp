
/*!
 * @file 
 * @brief 
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_CATEGORY_HPP
#define NYARUGA_UTIL_CATEGORY_HPP

#pragma once

#include <concepts>
#include <type_traits>
#include <functional>
#include <nyaruga_util/unwrap_template.hpp>

// 圏論的な型を集めました

namespace nyaruga {

namespace util {

namespace category {
   
   namespace detail {
      
      template <typename X, std::size_t N>
      constexpr auto unwrap_template_N_times_impl() noexcept
      {
         if constexpr (N == 0){ return std::type_identity<X>{}; }
         else return unwrap_template_N_times_impl<unwrap_template_idx<0, X>, N - 1>();
      }
      
      template <typename X>
      constexpr std::size_t unwrapable_times_one_arg_template_impl() noexcept
      {
         if constexpr (requires () { typename unwrap_template_idx<0, X>; })
            1 + unwrapable_times_one_arg_template_impl<unwrap_template_idx<0, X>>;
         else return 0;
      }
   }
                     
   template <typename X, std::size_t N>
   using unwrap_template_N_times = typename decltype(detail::unwrap_template_N_times_impl<X, N>())::type;
   
   template <typename X>
   constexpr std::size_t unwrapable_times_one_arg_template = detail::unwrapable_times_one_arg_template_impl<X>();
   
   namespace detail {

      template <typename X>
      concept is_unwrapable = requires() { typename unwrap_template_idx<0, X>; };

      template <template<class> class T, typename X, typename Tmp = X>
      constexpr unsigned int unwrapable_times_for_T_impl() noexcept
      {
         if constexpr (is_unwrapable<Tmp>)
            if constexpr (std::same_as<X, T<unwrap_template_idx<0, Tmp>>>)
               return 1;
            else
               return 1 + unwrapable_times_for_T_impl<T, X, unwrap_template_idx<0, Tmp>>();
         else return 0;
      }
   }
   
   template <template<class> class T, typename X>
   constexpr unsigned int unwrapable_times_for_T = detail::unwrapable_times_for_T_impl<T, X>();
   
   template <template<class> class T, typename X>
      requires(unwrapable_times_for_T<T, X> != 0)
   using unwrap_type_using_T = unwrap_template_N_times<X, unwrapable_times_for_T<T, X>>;
                     
   namespace detail {

      template <template<class> class T, typename X>
      concept is_unwrapable_with_T = requires(){ { std::declval<T<unwrap_type_using_T<T, X>>>() } -> std::convertible_to<X>; };
      
      template <template<class> class T, typename X>
      constexpr unsigned int T_rank_impl() noexcept
      {
         if constexpr (is_unwrapable_with_T<T, X>)
            return 1 + T_rank_impl<T, unwrap_type_using_T<T, X>>();
         else return 0;
      }
      
      template <template<class> class T, typename X, std::size_t N>
      constexpr auto apply_T_N_times_impl() noexcept
      {
         if constexpr(N == 0) return std::declval<X>();
         else return apply_T_N_times_impl<T, T<X>, N - 1>();
      }
   }

   // X が T< T< T< ... T<Y> ... > と再帰的に T が適用されているような型であるとき、
   // T が何回適用されているかを返す
   template <template<class> class T, typename X>
   constexpr unsigned int T_rank = detail::T_rank_impl<T, X>();
   
   template <typename Y, template <class> class T, typename X>
   concept same_T_rank_with = (T_rank<T, X> == T_rank<T, Y>);
   
   // T を型 X に N 回適用する。例：3回 X を適用すると T<T<T<X>>>
   template <template<class> class T, typename X, std::size_t N>
   using apply_T_N_times = decltype(detail::apply_T_N_times_impl<T, X, N>());

   // f : X -> Y
   template <typename Hom, typename Domain, typename Codomain>
   concept morphism_between = requires() { { std::declval<Hom>()(std::declval<Domain>()) } -> std::convertible_to<Codomain>; } ||
      requires(Hom f, Domain a) { { f(a) } -> std::convertible_to<Codomain>; };
   
   template <typename Hom, typename Domain>
   concept morphism_from = requires() { std::declval<Hom>()(std::declval<Domain>()); } ||
      requires(Hom f, Domain a) { f(a); };
   
   // (X, Y) to X -> Y
   template <typename X, typename Y>
   using make_morph = std::function<Y(X)>;
   
   template <typename Object>
   concept object = true;
   
   template <typename Object>
   concept domain = true;
   
   template <typename Object>
   concept codomain = true;
   
   template <domain X, morphism_from<X> Mor>
   using apply_morphism = decltype(std::declval<Mor>()(std::declval<X>()));
   
   // μ : TX -> X
   template <typename Mor, typename X,  template<class> class T>
   concept mu = morphism_from<Mor, X> && (T_rank<T, X> == 1) && (
      requires () { { std::declval<Mor>()(std::declval<X>()) } -> std::convertible_to<unwrap_type_using_T<T, X>>; } ||
      requires(Mor f, X x) { { f(x) } -> std::convertible_to<unwrap_type_using_T<T, X>>; }
   );

   template <template<class> class T, object X>
   using make_mu = make_morph<T<X>, X>;
   
   template<template<class> class T, object TX>
   requires requires () { std::convertible_to<TX, T<unwrap_type_using_T<T, TX>>>; }
   using apply_mu = unwrap_type_using_T<T, TX>;
   
   template <template <class> class T, typename X, typename Y>
   concept functor = requires(make_morph<X, Y> f, T<X> tx) { { (T<X>::fmap(f))(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { (tx.fmap(f))(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { (tx->fmap(f))(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { fmap(f)(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { fmap<X, Y>(f)(tx) } -> std::convertible_to<T<Y>>; };
   
   // join : TTX -> TX
   template <typename Mor, template <class> class T, typename Domain, typename Codomain>
   concept join = morphism_between<Mor, Domain, Codomain> && (
      requires() {
      typename apply_mu<T, apply_mu<T, Domain>>;
      { std::declval<Mor>()(std::declval<Domain>()) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; } ||
      requires(Mor f, Domain x) {
      typename apply_mu<T, apply_mu<T, Domain>>;
      { f(x) } ->std::convertible_to<T<apply_mu<T, Codomain>>>; });
   
   // Tf : TX -> TY
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept Tf = morphism_between<Mor, Domain, Codomain> &&
      requires () {
         std::convertible_to<Domain, T<apply_mu<T, Domain>>>;
         std::convertible_to<Codomain, T<apply_mu<T, Codomain>>>; };
   
   template <template<class> class T, object Domain, object Codomain>
   using make_Tf = make_morph<T<Domain>, T<Codomain>>;

   template <typename KleisliObject, template<class> class T>
   concept kleisli_object = (T_rank<T, KleisliObject> == 0);
   
   template <typename KleisliObject, template<class> class T>
   concept kleisli_domain = (T_rank<T, KleisliObject> == 0);
   
   template <typename KleisliObject, template<class> class T>
   concept kleisli_codomain = (T_rank<T, KleisliObject> == 1);

   // f : X -> TY
   template <typename KleisliMor, template<class> class T, typename Domain, typename Codomain>
   concept kleisli_morphism_between = morphism_between<KleisliMor, Domain, Codomain> && kleisli_domain<Domain, T> && (
      requires() { { std::declval<KleisliMor>()(std::declval<Domain>()) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; } ||
      requires(KleisliMor f, Domain x) { { f(x) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; });
   
   template <typename KleisliMor, template<class> class T, typename Domain>
   concept kleisli_morphism_from = morphism_from<KleisliMor, Domain> && kleisli_domain<Domain, T> && (
      requires() { { std::declval<KleisliMor>()(std::declval<Domain>()) } -> std::convertible_to<T<apply_mu<T, decltype(std::declval<KleisliMor>()(std::declval<Domain>()))>>>; } ||
      requires(KleisliMor f, Domain x) { { f(x) } -> std::convertible_to<T<apply_mu<T, decltype(f(x))>>>; });
   
   template <template<class> class T, kleisli_object<T> X, kleisli_object<T> Y>
   using make_kleisli_morph = make_morph<X, T<Y>>;
   
   template <template<class> class T, kleisli_object<T> X, kleisli_morphism_from<T, X> KleisliMor>
   using apply_kleisli_morph = decltype(std::declval<KleisliMor>()(std::declval<X>()));
   
   // kleisli_star = (-)* : (f : X -> TY) -> (f* : TX -> TY)
   template <template<class> class T, kleisli_object<T> X, kleisli_object<T> Y>
   using make_kleisli_star = make_morph<make_morph<X, T<Y>>, make_morph<T<X>, T<Y>>>;
                                       
   // f* : TX -> TY, in Haskell : (>>= f)
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept f_star = morphism_between<Mor, Domain, Codomain> && (T_rank<T, Domain> == 1) && (T_rank<T, Codomain> == 1) && (
   requires () { { std::declval<Mor>()(std::declval<T<apply_mu<T, Domain>>>()) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; } ||
   requires (Mor fstar, T<apply_mu<T, Domain>> x) { { fstar(x) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; });

   template <template<class> class T, kleisli_object<T> X, kleisli_object<T> Y>
   using make_f_star = make_morph<T<X>, T<Y>>;
   
   // mu_T : (μ◦T(-))(-), in Haskell : >>=
   template <template<class> class T, kleisli_object<T> X, kleisli_object<T> Y>
   using make_mu_T = std::function<T<Y>(T<X>, make_morph<X, T<Y>>)>;
   
   template <template<class> class T, typename X, typename Y>
   concept has_mu_T = kleisli_object<X, T> && kleisli_object<Y, T> && (
      requires (T<X> x, category::make_morph<X, T<Y>> f) {
         { std::declval<T<X>>() >= std::declval<category::make_morph<X, T<Y>>>() } -> std::convertible_to<T<Y>>; } ||
      requires (T<X> x, category::make_morph<X, T<Y>> f) {
         { x >= f } -> std::convertible_to<T<Y>>; });
   
   template <template <class> class T, typename X, typename Y>
   concept monad = has_mu_T<T, X, Y> && kleisli_object<X, T> && kleisli_object<Y, T> &&
   (
      requires (X x, Y y) {
         { T<X>::ret(x) } -> std::convertible_to<T<X>>;
         { T<Y>::ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) {
         { tx.ret(x) } -> std::convertible_to<T<X>>;
         { ty.ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) {
         { tx->ret(x) } -> std::convertible_to<T<X>>;
         { ty->ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) {
         { ret(x) } -> std::convertible_to<T<X>>;
         { ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) {
         { ret<X>(x) } -> std::convertible_to<T<X>>;
         { ret<Y>(y) } -> std::convertible_to<T<Y>>; } ||
      requires () {
         { T<X>::ret(std::declval<X>()) } -> std::convertible_to<T<X>>;
         { T<Y>::ret(std::declval<Y>()) } -> std::convertible_to<T<Y>>; } ||
      requires (T<X> tx, T<Y> ty) {
         { tx.ret(std::declval<X>()) } -> std::convertible_to<T<X>>;
         { ty.ret(std::declval<Y>()) } -> std::convertible_to<T<Y>>; } ||
      requires (T<X> tx, T<Y> ty) {
         { tx->ret(std::declval<X>()) } -> std::convertible_to<T<X>>;
         { ty->ret(std::declval<Y>()) } -> std::convertible_to<T<Y>>; } ||
      requires (T<X> tx, T<Y> ty) {
         { ret(std::declval<X>()) } -> std::convertible_to<T<X>>;
         { ret(std::declval<Y>()) } -> std::convertible_to<T<Y>>; } ||
      requires (T<X> tx, T<Y> ty) {
         { ret<X>(std::declval<X>()) } -> std::convertible_to<T<X>>;
         { ret<Y>(std::declval<Y>()) } -> std::convertible_to<T<Y>>; }
   );
   
   // η : X -> TX, in Haskell : return
   // 名前の衝突を避けるためここに
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept ret = morphism_between<Mor, Domain, Codomain> && kleisli_domain<Domain, T> &&
      kleisli_codomain<Codomain, T> && std::convertible_to<T<Domain>, Codomain>;
   
   template <template<class> class T, kleisli_object<T> X>
   using apply_ret = T<X>;
   
   template <template<class> class T, kleisli_object<T> X>
   using make_ret = make_morph<X, T<X>>;
   
} // namespare category

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
