//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CARTESIAN_PRODUCT_HPP
#define NYARUGA_UTIL_CARTESIAN_PRODUCT_HPP

#pragma once

// this code almost from
// https://stackoverflow.com/questions/16686942/how-to-iterate-over-two-stl-like-containers-cartesian-product

// ネストした for 文が書きやすくなります
// 速度はブレ幅が大きいですが、for 文をネストした場合より 2 倍以上遅くなることはないと思います
// パソコンの調子によって、1.5 倍くらいだったり、同じくらいだったりしました

// 例

/*
    array b = { false, true };
    array i = { 0, 1 };
    array s = { "Hello", "World" };

    // i, j, k は、それぞれの添え字に関してネストした for 文と
    // 同じような添え字を走る(直積)
    // 引数はイテレータが使える型や組み込みの配列なら OK
    // (使用する操作：std::end(), std::begin(), it++) 
    for (auto&& [i,j,k]: cartesian_product(b,i,s)) 
    {
        std::cout << std::boolalpha << i << ", ";
        std::cout << j << ", ";
        std::cout << k << "\n";
    } 

    //   for (int i = 0; i != 2; ++i)
    //   for (int j = 1; j != 3; ++j) 
    //   for (int k = 2; k != 4; ++k) 
   for (auto&& [i,j,k]: cartesian_product({0,2},{1,3},{2,4})) 
    {
        std::cout << i << ", ";
        std::cout << j << ", ";
        std::cout << k << "\n";
    }

*/

#include <tuple>                        // make_tuple, tuple
#include <utility>                      // pair
#include <vector>                       // vector
#include <boost/coroutine2/coroutine.hpp>

namespace nyaruga::util {

namespace impl1 { // コルーチン実装

namespace detail {

// the lambda is fully bound with one element from each of the ranges
template<class Op>
void insert_tuples(Op&& op) noexcept
{
    // evaluating the lambda will insert the currently bound tuple
    op();
}

// "peal off" the first range from the remaining tuple of ranges
template<class Op, class InputIterator1, class... InputIterator2>
void insert_tuples(Op&& op, std::pair<InputIterator1, InputIterator1> head, std::pair<InputIterator2, InputIterator2>... tail)
{
    // "peal off" the elements from the first of the remaining ranges
    // NOTE: the recursion will effectively generate the multiple nested for-loops
    for (auto it = head.first; it != head.second; ++it) {
        // bind the first free variable in the lambda, and
        // keep one free variable for each of the remaining ranges
        detail::insert_tuples(
            [&op,&it](InputIterator2... elems) { op(it, elems...); },
            tail...
        );
    }
}

}   // namespace detail

// convert a tuple of ranges to the range of tuples representing the Cartesian product
template<class... InputIterator>
void cartesian_product_impl(auto & sink, std::pair<InputIterator, InputIterator>&&... dimensions)
{
    detail::insert_tuples(
         [&sink](InputIterator... elems) { sink(std::make_tuple(*elems...)); },
         std::forward<decltype(dimensions)>(dimensions)...
    );
}
    
template<class...Args>
decltype(auto) cartesian_product(Args&&... args)
{
    using coro_type = boost::coroutines2::coroutine<
        typename std::tuple<std::remove_reference_t<decltype(*std::begin(std::declval<Args&>()))>...> 
    >;
    
    return typename coro_type::pull_type(
    [...args=std::forward<Args>(args)](typename coro_type::push_type & sink)
    {
        cartesian_product_impl(sink, std::make_pair(std::begin(args), std::end(args))...);   
    });
};
} //namespace impl1

// from: https://stackoverflow.com/questions/16686942/how-to-iterate-over-two-stl-like-containers-cartesian-product
namespace impl2 { // ほぼ stack overflow から 

namespace detail {

// the lambda is fully bound with one element from each of the ranges
template<class Op>
void insert_tuples(Op&& op)
{
        // evaluating the lambda will insert the currently bound tuple
        op();
}

// "peal off" the first range from the remaining tuple of ranges
template<class Op, class InputIterator1, class... InputIterator2>
void insert_tuples(Op&& op, std::pair<InputIterator1, InputIterator1> head, std::pair<InputIterator2, InputIterator2>... tail)
{
        // "peal off" the elements from the first of the remaining ranges
        // NOTE: the recursion will effectively generate the multiple nested for-loops
        for (auto it = head.first; it != head.second; ++it) {
                // bind the first free variable in the lambda, and
                // keep one free variable for each of the remaining ranges
                detail::insert_tuples(
                        [&op,&it](InputIterator2... elems) mutable { op(it, elems...); },
                        tail...
                );
        }
}

// convert a tuple of ranges to the range of tuples representing the Cartesian product
template<class OutputIterator, class... InputIterator>
void cartesian_product_impl(OutputIterator result, std::pair<InputIterator, InputIterator>... dimensions)
{
        insert_tuples(
                 [=](InputIterator... elems) mutable { *result++ = std::make_tuple(*elems...); },
                 dimensions...
        );
}

}       // namespace detail



template <typename...Args>
auto cartesian_product(Args&&...args)
{
    std::vector< typename std::tuple<std::remove_reference_t<decltype(*std::begin(std::declval<Args&>()))>...>  > result;
    detail::cartesian_product_impl(
            std::back_inserter(result),
            std::make_pair(std::begin(args), std::end(args))...
    );
    return result;
}

} // namespace impl2 (almost original)

using namespace impl2; // impl1 より倍速い場合があった 速度はパソコンの調子によって大きく左右される

} // namespace nyaruga::util

/* how to use

#include <string>
#include <iostream>
#include <array>
#include <stdio.h>
#include <chrono>

using std::array;

int main() 
{
    array b = { false, true };
    array i = { 0, 1 };
    array s = { "Hello", "World" };
    
    
    using namespace std;
    chrono::system_clock::time_point start, end;

    start = chrono::system_clock::now();

    for (int f = 0; f < 1000; ++f)
    // now use a single flat loop over result to do your own thing
    for (auto&& [i,j,k]: cartesian_product(b,i,s)) {
        std::cout << std::boolalpha << i << ", ";
        std::cout << j << ", ";
        std::cout << k << "\n";
    }
    
        // 何かの処理

    end = chrono::system_clock::now();

    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);
    
}   

*/

#include <initializer_list>

namespace nyaruga::util {

namespace detail_index {

template<class Op>
void insert_tuples(Op&& op)
{
        op();
}

void insert_tuples(auto&& op,auto head, auto... tail)
{
    for (auto i = *head.begin(); i < *(head.begin()+1); ++i) {
        insert_tuples(
            [&op,&i](typename decltype(tail)::value_type... elems) mutable { op(i, elems...); },
            tail...
        );
    }
}

void cartesian_product_impl(auto&& result, auto&&... dimensions)
{
    insert_tuples(
         [=](auto... elems) mutable { *result++ = std::make_tuple(elems...); },
         std::forward<decltype(dimensions)>(dimensions)...
    );
}

}       // namespace detail

template<typename... T>
auto cartesian_product(std::initializer_list<T>...args)
{
    std::vector<typename std::tuple<T...>> result;
    detail_index::cartesian_product_impl(std::back_inserter(result), args...);
    return result;
}

}

/* how to use
#include <string>
#include <iostream>
#include <array>
#include <stdio.h>
#include <chrono>

using std::array;

int main() 
{
    
    using namespace std;
    chrono::system_clock::time_point start, end;

    start = chrono::system_clock::now();

    for (int f = 0; f < 1000; ++f)
    
   for (auto&& [i,j,k]: cartesian_product({0,2},{1,3},{2,4})) 
//   equal to
//   for (int i = 0; i != 2; ++i)
//   for (int j = 1; j != 3; ++j) 
//   for (int k = 2; k != 4; ++k) 
    {
        std::cout << i << ", ";
        std::cout << j << ", ";
        std::cout << k << "\n";
    }
    
    end = chrono::system_clock::now();

    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);

}  
*/

#endif // NYARUGA_UTPL_CARTESIAN_PRODUCT_HPP