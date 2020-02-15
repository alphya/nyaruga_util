//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CATEGORY_HPP
#define NYARUGA_UTIL_CATEGORY_HPP

#pragma once

#include <concepts>
#include <nyaruga_util/unwrap_template_idx.hpp>

// 圏論的な型を集めました

namespace nyaruga::util {

namespace category {
   
   namespace detail {
      template <template<class> class T, typename X>
      constexpr unsigned int T_rank_impl()
      {
         if constexpr (requires(){ { std::declval<T<unwrap_template_idx<0, X>>>() } -> std::convertible_to<X>; })
            return 1 + T_rank_impl<T, unwrap_template_idx<0, X>>();
         else return 0;
      }
   }
   
   // X が T< T< T< ... T<Y> ... > と再帰的に T が適用されているような型であるとき、
   // T が南海適用されているかを返す
   template <template<class> class T, typename X>
   constexpr unsigned int T_rank = detail::T_rank_impl<T, X>();
   
   template <typename Y, template <class> class T, typename X>
   concept same_T_rank_with = (T_rank<T, X> == T_rank<T, Y>);

   // f : X -> Y
   template <typename Hom, typename Domain, typename Codomain>
   concept morphism_between = requires(Hom f, Domain a) { { f(a) } -> std::convertible_to<Codomain>; };
   
   template <typename Hom, typename Domain>
   concept morphism_from = requires(Hom f, Domain a) { f(a); };
   
   // (X, Y) to X -> Y
   template <typename X, typename Y>
   using make_morph = Y(*)(X);
    
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
   concept mu = morphism_from<Mor, X> && (T_rank<T, X> == 1) && 
      requires (Mor f, X x) { { f(x) } -> std::convertible_to<unwrap_template_idx<0, X>>; };

   template <template<class> class T, object X>
   using make_mu = make_morph<T<X>, X>;
    
   template<template<class> class T, object TX>
   requires requires () { std::convertible_to<TX, T<unwrap_template_idx<0, TX>>>; }
   using apply_mu = unwrap_template_idx<0, TX>;
   
   template <template <class> class T, typename X, typename Y>
   concept functor = requires(make_morph<X, Y> f, T<X> tx) { { (T<X>::fmap(f))(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { (tx.fmap(f))(tx) } -> std::convertible_to<T<Y>>; } ||
                     requires(make_morph<X, Y> f, T<X> tx) { { (tx->fmap(f))(tx) } -> std::convertible_to<T<Y>>; };
   
   // join : TTX -> TX
   template <typename Mor, template <class> class T, typename Domain, typename Codomain>
   concept join = morphism_between<Mor, Domain, Codomain> && requires(Mor f, Domain x) { 
      typename apply_mu<T, apply_mu<T, Domain>>;
      { f(x) } -> std::convertible_to<T<apply_mu<T, Codomain>>>;                                 
   };
   
   // η : X -> TX, in Haskell : return
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept ret = morphism_between<Mor, Domain, Codomain> && (T_rank<T, Domain> == 0) &&
      requires(Mor f, Domain x) { 
         { f(x) } -> std::convertible_to<T<Codomain>>;
         std::convertible_to<T<Domain>, Codomain>; };
   
   template <template<class> class T, object X>
   using apply_ret = T<X>;
    
   template <template<class> class T, object X>
   using make_ret = make_morph<X, T<X>>;
   
   // Tf : TX -> TY
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept Tf = morphism_between<Mor, Domain, Codomain> && 
      requires () { 
         std::convertible_to<Domain, T<apply_mu<T, Domain>>>;
         std::convertible_to<Codomain, T<apply_mu<T, Codomain>>>; };
    
   template <template<class> class T, object Domain, object Codomain>
   using make_Tf = make_morph<T<Domain>, T<Codomain>>;

   // f : X -> TY
   template <typename KleisliMor, template<class> class T, typename Domain, typename Codomain>
   concept kleisli_morphism = morphism_between<KleisliMor, Domain, Codomain> && (T_rank<T, Domain> == 0) &&
       requires(KleisliMor f, Domain x) { { f(x) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; };
   
   template <typename KleisliMor, template<class> class T, typename Domain>
   concept kleisli_morphism_from = morphism_from<KleisliMor, Domain> && (T_rank<T, Domain> == 0) &&
   requires(KleisliMor f, Domain x) { { f(x) } -> std::convertible_to<T<apply_mu<T, decltype(f(x))>>>; };
   
   template <template<class> class T, object X, object Y>
   using make_kleisli_morph = make_morph<X, T<Y>>;
   
   template <template<class> class T, object X, kleisli_morphism_from<T, X> KleisliMor>
   using apply_kleisli_morph = decltype(std::declval<KleisliMor>()(std::declval<X>()));
   
   // kleisli_star = (-)* : (f : X -> TY) -> (f* : TX -> TY)                                           
   template <template<class> class T, object X, object Y>
   using make_kleisli_star = make_morph<make_morph<X, T<Y>>, make_morph<T<X>, T<Y>>>;
                                       
   // f* : TX -> TY, in Haskell : (>>= f)
   template <typename Mor, template<class> class T, typename Domain, typename Codomain>
   concept f_star = morphism_between<Mor, Domain, Codomain> && (T_rank<T, Domain> == 1) &&
      requires (Mor fstar, T<apply_mu<T, Domain>> x) 
   { { fstar(x) } -> std::convertible_to<T<apply_mu<T, Codomain>>>; };

   template <template<class> class T, object X, object Y>
   using make_f_star = make_morph<T<X>, T<Y>>;
   
   // mu_T : (μ◦T(-))(-), in Haskell : >>=
   template <template<class> class T, object X, object Y>
      requires (T_rank<T, X> == 0) && (T_rank<T, Y> == 0)
   using make_mu_T = T<Y>(*)(T<X>, make_morph<X, T<Y>>);
   
   template <template<class> class T, typename X, typename Y>
   concept has_mu_T = (T_rank<T, X> == 0) && (T_rank<T, Y> == 0) &&
      requires (T<X> x, category::make_morph<X, T<Y>> f) { 
         { x >> f } -> std::convertible_to<T<Y>>; 
   };
   
   template <template <class> class T, typename X, typename Y>
   concept monad = functor<T, X, Y> && has_mu_T<T, X, Y> &&
   (
      requires (X x, Y y) { 
         { T<X>::ret(x) } -> std::convertible_to<T<X>>;
         { T<Y>::ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) { 
         { tx.ret(x) } -> std::convertible_to<T<X>>;
         { ty.ret(y) } -> std::convertible_to<T<Y>>; } ||
      requires (X x, Y y, T<X> tx, T<Y> ty) { 
         { tx->ret(x) } -> std::convertible_to<T<X>>;
         { ty->ret(y) } -> std::convertible_to<T<Y>>; }
   );
   
} // namespare category

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
