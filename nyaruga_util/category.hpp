//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CATEGORY_HPP
#define NYARUGA_UTIL_CATEGORY_HPP

#pragma once

#include <concepts>
#include <unwrap_helper_idx.hpp>

// 圏論的な型を集めました

namespace nyaruga::util {

namespace category {

   // μ : TX -> X
   template <template<class> class T, typename X>
   using mu = std::remove_cvref_t<X>(*)(const T<std::remove_cvref_t<X>>&);
   
   // η : X -> TX, in Haskell : return
   template <template<class> class T, typename X>
   using eta = T<std::remove_cvref_t<X>>(*)(const X&);
   
   // Tf : TX -> TY
   template <typename Mor, template<class> class T, typename X>
   requires requires (Mor f, X x) { f(x); }
   using Tf = T<std::remove_cvref_t<decltype(std::declval<Mor>()(std::declval<std::remove_cvref_t<X>>()))>>(*)(const T<std::remove_cvref_t<X>>&);
   
   // T : f -> Tf (f : X -> Y)
   template <typename Mor, template<class> class T, typename X>
   requires requires (Mor f, X x) { f(x); }
   using functor = Tf<Mor, T, std::remove_cvref_t<X>>(*)(const Mor&);
    
   // (-)* : (f : X -> TY) -> (f* : TX -> TY)
   template <typename KleisliMor, template<class> class T, typename X>
   requires requires (KleisliMor f, X x) { { f(x) } -> std::same_as<T<unwrap_helper_idx<0, decltype(f(x))>>>;  }
   using star = T<unwrap_helper_idx<0, decltype(std::declval<KleisliMor>()(std::declval<X>()))>>(* (*)(const KleisliMor&))(const T<X>&);
                                                                        
   // f* : TX -> TY, in Haskell : (>>= f)
   template <typename KleisliMor, template<class> class T, typename X>
   requires requires (KleisliMor f, T<std::remove_cvref_t<X>> x) { f(std::declval<mu<T, X>>()(x)); }
   using f_star = T<std::remove_cvref_t<unwrap_helper_idx<0, decltype(std::declval<KleisliMor>()(std::declval<X>()))>>> (*)(const T<std::remove_cvref_t<X>>&);
    
   // mu_T : (μ◦T(-))(-), in Haskell : >>=
   template <typename KleisliMor, template<class> class T, typename X>
   requires requires (KleisliMor f, T<std::remove_cvref_t<X>> x) { f(std::declval<mu<T, X>>()(x)); }
   using mu_T = decltype(std::declval<KleisliMor>()(std::declval<X>()))(*)(const T<std::remove_cvref_t<X>>&, const KleisliMor&);

} // namespare category

/* 表現の例

template <typename T>
using F = std::pair<T, int>;

using namespace category;
mu<F, int> m = [](const F<int>& a){ return a.first; };
eta<F, int> et = [](const int& a){ return F<int>{a, a*3}; };
Tf<decltype([](const int&)->double{}), F, int> tf = [](const F<int>& a){ return F<double>{a.first, a.second*3}; };
functor<double(*const&)(const int&), F, int> funct = [](double(*const&)(const int&))->F<double>(*)(const F<int>&){ return [](const F<int>& a){ return F<double>{a.first, a.second*3}; }; };
star<F<double>(*const&)(const int&), F, int> st = [](F<double>(*const&)(const int&))->F<double>(*)(const F<int>&){ return [](const F<int>& a){ return F<double>{a.first, a.second*3}; }; };
f_star<F<double>(*const&)(const int&), F, int> fst = [](const F<int>& a){ return F<double>{a.first, a.second*3}; };
mu_T<F<double>(*const&)(const int&), F, int> mut = [](const F<int>&, F<double>(*const& f)(const int&)){ return F<double>{}; };

*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_CATEGORY_HPP
