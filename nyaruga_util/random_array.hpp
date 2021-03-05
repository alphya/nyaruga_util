
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2020-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

// ボレロ村上さんのブログ
// http://boleros.hateblo.jp/entry/20110930/1317355718 より

/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef NYARUGA_UTIL_RANDOM_ARRAY_HPP
#define NYARUGA_UTIL_RANDOM_ARRAY_HPP

#pragma once

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <sprout/array.hpp>
#include <sprout/random.hpp>
#include <sprout/random/unique_seed.hpp>

namespace nyaruga {

namespace util {

//
// random_array
//
//  乱数生成器 Gen を使って乱数値の配列を生成する。
//
template<typename T, std::size_t N, typename Gen>
constexpr sprout::array<T, N> random_array(Gen const& gen);

//
// random_array_impl
//
//  要素数が指定の数に達した場合、配列を生成して返す。
//
template<typename T, std::size_t N, typename Random, typename... Args>
constexpr typename std::enable_if<
    sizeof...(Args) == N - 1,
    sprout::array<T, N>
>::type random_array_impl(Random const& rnd, Args const&... args) {
    return sprout::array<T, N>{{args..., *rnd}};
}
//
//  要素数が指定未満の場合、乱数値を生成して再帰。
//  Random は、乱数生成器が返す結果型。「生成された乱数値」と「次の状態の乱数生成器」が格納されている。
//  constexpr では関数型言語のように状態の変更が出来ないため、このような仕様になっている。
//
template<typename T, std::size_t N, typename Random, typename... Args>
constexpr typename std::enable_if<
    sizeof...(Args) < N - 1,
    sprout::array<T, N>
>::type random_array_impl(Random const& rnd, Args const&... args) {
    //
    //  operator*() で、「生成された乱数値」を取得。
    //  operator()() で、「次の状態の乱数生成器」を呼び出す。
    //
    return random_array_impl<T, N>(rnd(), args..., *rnd);
}

//
// random_array の実装
//
//  乱数生成器の最初の呼出。
//
template<typename T, std::size_t N, typename Gen>
constexpr sprout::array<T, N> random_array(Gen const& gen) {
    return random_array_impl<T, N>(gen());
}

/* 使用方法

int main() {
    //
    //  SPROUT_UNIQUE_SEED は constexpr として使える、ほぼユニークかつコンパイル毎に変化する std::size_t 値になる。
    //  実装は、日時とファイル名と行の文字列からハッシュ値を生成している。
    //
    static constexpr std::size_t seed = SPROUT_UNIQUE_SEED;
    //
    //  minstd_rand0 は 線形合同法エンジンのジェネレータ。
    //
    static constexpr auto engine = sprout::random::minstd_rand0(seed);
    //
    //  uniform_smallint は 整数の一様分布。
    //  ここでは [1 .. 6] の範囲。
    //
    static constexpr auto distribution = sprout::random::uniform_smallint<int>(1, 6);
    //
    //  random_array を使って乱数値の配列を生成。
    //  combine は、ジェネレータと分布を一つの乱数生成器（variate_generator）にまとめる。
    //
    static constexpr auto arr = random_array<int, 10>(sprout::random::combine(engine, distribution));

    //
    //  配列を標準出力に出力。
    //
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}

*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_RANDOM_ARRAY_HPP
