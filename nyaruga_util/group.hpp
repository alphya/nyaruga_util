
/*!
 * @file 
 * @brief Defines configuration macros used throughout the library.
 * @copyright alphya 2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_GROUP_HPP
#define NYARUGA_UTIL_GROUP_HPP

#include <nyaruga_util/algebra.hpp>
#include <Eigen/Dense>
#include <complex>
#include <cmath>
#include <concepts>
#include <numbers>

namespace nyaruga {

namespace util {

namespace detail {

template <std::floating_point T = double>
struct SU1
{
    using value_type = std::complex<T> ;
private:
    std::complex<T> val_;
    constexpr SU1(const std::complex<T>& v) noexcept : val_(v) {}
public:
    constexpr void set_val(const T& theta) noexcept { val_ = exp(std::complex<T>{0,-1}*theta); }
    constexpr std::complex<T> val(const T& theta) const noexcept { val_  = exp(std::complex<T>{0,-1}*theta); return val_; }
    constexpr std::complex<T> val() const noexcept{ return val_; }
    constexpr operator std::complex<T>() const noexcept{ return val_; }
    constexpr SU1() noexcept = default;
    constexpr SU1(const SU1& v) noexcept : val_(v.val_) {}
    constexpr SU1(const T& theta) noexcept : val_(exp(std::complex<T>{0,-1}*theta)) {}
    friend SU1 operator*(const SU1& lhs, const SU1 & rhs) noexcept { return SU1(lhs.val()*rhs.val()); }
    constexpr SU1 conj() { return std::conj(val_); }
};

template <std::floating_point T = double>
struct SO2
{
    using value_type = Eigen::Matrix<T,2,2> ;
private:
    Eigen::Matrix<T,2,2> val_;
    constexpr SO2(const Eigen::Matrix<T,2,2>& v) noexcept : val_(v) {}
public:
    constexpr void set_val(const T& theta) noexcept { val_ << cos(theta), -sin(theta), sin(theta), cos(theta); }
    constexpr Eigen::Matrix<T,2,2> val(const T& theta) const noexcept { val_ << cos(theta), -sin(theta), sin(theta), cos(theta); return val_; };
    constexpr Eigen::Matrix<T,2,2> val() const noexcept{ return val_; };
    constexpr operator Eigen::Matrix<T,2,2>() const noexcept { return val_; }
    constexpr SO2() noexcept = default;
    constexpr SO2(const SO2& v) noexcept : val_(v.val_) {}
    constexpr SO2(const T& theta) noexcept { val_ << cos(theta), -sin(theta), sin(theta), cos(theta);}
    friend SO2 operator*(const SO2& lhs, const SO2 & rhs) noexcept { return SO2(lhs.val()*rhs.val()); }
    constexpr SO2 transpose() { return SO2(val_.transpose()); }
};

template <std::floating_point T = double>
struct SO3 // http://www.tuhep.phys.tohoku.ac.jp/~watamura/kougi/GP2012_4.pdf
{
    using value_type = Eigen::Matrix<T,3,3> ;
private:
    Eigen::Matrix<T,3,3> val_;
    constexpr SO3(const Eigen::Matrix<T,3,3>& v) noexcept : val_(v) {}

    // alpha, beta, gamma はオイラー角
    constexpr value_type calc_val(const T& alpha, const T& beta, const T& gamma) noexcept
    {
        value_type RX, RY, RZ;
        RX << 1, 0, 0,  0, cos(alpha), -sin(alpha),  0, sin(alpha), cos(alpha);
        RY << cos(beta), 0, sin(beta),  0, 1, 0,  -sin(beta), 0, cos(beta);
        RZ << cos(gamma), -sin(gamma), 0,  sin(gamma), cos(gamma), 0,  0, 0, 1;
        return RX*(RY*RZ);
    }

public:
    // alpha, beta, gamma はオイラー角
    constexpr void set_val(const T& alpha, const T& beta, const T& gamma) noexcept { val_ = calc_val(alpha, beta, gamma); }
    constexpr value_type val(const T& alpha, const T& beta, const T& gamma) const noexcept { val_ = calc_val(alpha, beta, gamma); return val_; };
    constexpr value_type val() const noexcept{ return val_; };
    constexpr operator value_type() const noexcept { return val_; }
    constexpr SO3() noexcept = default;
    constexpr SO3(const SO3& v) noexcept : val_(v.val_) {}
    constexpr SO3(const T& alpha, const T& beta, const T& gamma) noexcept : val_(calc_val(alpha, beta, gamma)) {}
    friend SO3 operator*(const SO3& lhs, const SO3 & rhs) noexcept { return SO3(lhs.val()*rhs.val()); }
    constexpr SO3 transpose() { return SO3(val_.transpose()); }
};

} // detail

using detail::SU1;
using detail::SO2;
using detail::SO3;

} // util

} // nyaruga

/* how to use

# include "group.hpp"

using namespace nyaruga::util;
using namespace std::numbers;

int main(){
    SU1 a(pi/2*23);
    SU1 b(pi/32);
    std::cout << norm((a*b).val()) << "\n"; // 1
    std::cout << (a.conj()*(a)).val() << "\n\n"; // E
    
    SO2 c(pi/2+2);
    SO2 d(pi/2*24+4);
    std::cout << (c*d).val() << "\n";
    std::cout << (c*c.transpose()).val() << "\n"; // E
    std::cout << (c*d).val().determinant() << "\n\n"; //1

    SO3 e(pi/2+2, 23., 4.);
    SO3 f(pi/2*24+4, 3., 42.);
    std::cout << (e*f).val() << "\n";
    std::cout << (e*e.transpose()).val() << "\n"; // E
    std::cout << (e*f).val().determinant() << "\n"; //1
}

*/


#endif // #ifndef NYARUGA_UTIL_GROUP_HPP