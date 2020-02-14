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
       template <typename Ret, typename Arg>
       auto morphism_impl(Ret(*const& f)(Arg)) -> void;
       
       template <typename Ret, typename Arg>
       auto domain_impl(Ret(*const& f)(Arg)) -> std::remove_cvref_t<Arg>;
       
       template <typename Ret, typename Arg>
       auto codomain_impl(Ret(*const& f)(Arg)) -> std::remove_cvref_t<Ret>;
   }
    
   // f : X -> Y
   template <typename Hom>
   concept morphism = requires(Hom f) { detail::morphism_impl(f); }; // for compiler bug
   
   // (X, Y) to X -> Y
   template <typename X, typename Y>
   using make_morph = Y(*)(X);
    
   template <typename Ob>
   concept object = true;
    
   template <typename X>
   concept domain = true;
    
   template <typename Y>
   concept codomain = true;
    
   template <morphism Hom>
   using domain_of = decltype(detail::domain_impl(std::declval<Hom>()));
    
   template <morphism Hom>
   using codomain_of = decltype(detail::codomain_impl(std::declval<Hom>()));
   
   // μ : TX -> X
   template <typename Mor,  template<class> class T>
   concept mu = morphism<Mor> && 
      requires (Mor f, domain_of<Mor> x) { { f(x) } -> std::same_as<codomain_of<Mor>>; };

   template <template<class> class T, object X>
   using make_mu = make_morph<T<X>, X>;
    
   template<template<class> class T, object TX>
   requires requires () { std::same_as<TX, T<unwrap_template_idx<0, TX>>>; }
   using apply_mu = unwrap_template_idx<0, TX>;
   
   template <template <class> class T, typename X, typename Y>
   concept functor = requires() { { T<X>::fmap(std::declval<make_morph<X, Y>>()) } -> std::same_as<make_morph<T<X>, T<Y>>>; };
   
   // join : TTX -> TX
   template <typename Mor, template <class> class T>
   concept join = morphism<Mor> && requires(Mor f, domain_of<Mor> x) { 
      typename apply_mu<T, apply_mu<T, domain_of<Mor>>>;
      { f(x) } -> std::same_as<T<apply_mu<T, codomain_of<Mor>>>>;                                 
   };
   
   // η : X -> TX, in Haskell : return
   template <typename Mor, template<class> class T>
   concept ret = morphism<Mor> && requires(Mor f, domain_of<Mor> x) { { f(x) } -> std::same_as<T<codomain_of<Mor>>>;
                                                                           std::same_as<T<domain_of<Mor>>, codomain_of<Mor>>; };
   
   template <template<class> class T, object X>
   using apply_ret = T<X>;
    
   template <template<class> class T, object X>
   using make_ret = make_morph<X, T<X>>;
   
      // Tf : TX -> TY
   template <typename Mor, template<class> class T>
   concept Tf = morphism<Mor> && requires () { std::same_as<domain_of<Mor>, T<apply_mu<T, domain_of<Mor>>>>;
                                               std::same_as<codomain_of<Mor>, T<apply_mu<T, codomain_of<Mor>>>>; };
    
   // T : f -> Tf (f : X -> Y)
   template <template<class> class T, morphism Mor>
   using make_Tf = make_morph<T<domain_of<Mor>>, T<codomain_of<Mor>>>;

   // f : X -> TY
   template <typename KleisliMor, template<class> class T>
   concept kleisli_morphism = morphism<KleisliMor> && 
       requires(KleisliMor f, domain_of<KleisliMor> x) { { f(x) } -> std::same_as<T<apply_mu<T, codomain_of<KleisliMor>>>>; };
   
   template <category::morphism Mor, template<class> class T>
   using morph_to_kleisli_morph = make_morph<domain_of<Mor>, T<codomain_of<Mor>>>;
   
   // (-)* : (f : X -> TY) -> (f* : TX -> TY)
   template <typename Star, template<class> class T>
   concept kleisli_star = morphism<Star> && requires(Star star) 
   {  // 引数は射 X -> TY
      { std::declval<domain_of<Star>>()(std::declval<domain_of<domain_of<Star>>>()) }
         -> std::same_as<T<apply_mu<T, codomain_of<domain_of<Star>>>>>; 
      // 戻り値は射 TX -> TY
      { std::declval<codomain_of<Star>>()(std::declval<T<apply_mu<T, domain_of<codomain_of<Star>>>>>()) }
         -> std::same_as<T<apply_mu<T, codomain_of<domain_of<Star>>>>>;
   };
                                                    
   template <template<class> class T, morphism Mor>
   using make_kleisli_star = make_morph<make_morph<domain_of<Mor>, T<codomain_of<Mor>>>, 
                                        make_morph<T<codomain_of<Mor>>, T<codomain_of<Mor>>>>;
   
   template<template<class> class T, kleisli_morphism<T> Mor>
   using apply_klisli_star = make_morph<T<domain_of<Mor>>, codomain_of<Mor>>;
                                       
   // f* : TX -> TY, in Haskell : (>>= f)
   template <typename Mor, template<class> class T>
   concept f_star = morphism<Mor> && requires (Mor fstar) { { fstar(std::declval<T<apply_mu<T, domain_of<Mor>>>>()) }
                                                                 -> std::same_as<T<apply_mu<T, codomain_of<Mor>>>>; };

   template <template<class> class T, kleisli_morphism<T> KleisliMor>
   using make_f_star = make_morph<codomain_of<KleisliMor>, T<std::remove_cvref_t<domain_of<KleisliMor>>>>;
   
   // mu_T : (μ◦T(-))(-), in Haskell : >>=
   template <typename Mu_T, template<class> class T, typename KleisliMor, typename X>
   concept mu_T = kleisli_morphism<KleisliMor, T> && requires (Mu_T mu_t, KleisliMor f, X x) {
      typename apply_mu<T, X>;
      { mu_t(x, f) } -> std::same_as<codomain_of<KleisliMor>>;
   };
   
   template <template<class> class T, kleisli_morphism<T> KleisliMor>
   using make_mu_T = codomain_of<KleisliMor>(*)(T<domain_of<KleisliMor>>, KleisliMor);
   
   template <template <class> class T, typename X, typename Y>
   concept monad = functor<T, X, Y> && requires (typename T<X>:: template ret<make_morph<X, Y>, T> f) 
   { { f(std::declval<X>()) } -> std::same_as<T<Y>>; 
     { std::declval<T<X>>(). template operator() <T, make_morph<X, T<Y>>>(std::declval<T<X>>, std::declval<make_morph<X, T<Y>>>()) } -> std::same_as<T<Y>>;                                                                                              
   };
   
   
} // namespare category

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
