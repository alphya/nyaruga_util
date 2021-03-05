
/*!
 * @file 
 * @brief 
 * @copyright alphya 2018-2021
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef NYARUGA_UTIL_PRIVATE_ACCESSOR_HPP
#define NYARUGA_UTIL_PRIVATE_ACCESSOR_HPP

#pragma once

// 元となるコード
// https://gist.github.com/1528856
// Accessing Private Data
// c.f.
// http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
// c.f. http://d.hatena.ne.jp/redboltz/touch/20120111/1326292284

namespace nyaruga {

namespace util {

namespace private_accessor_impl {

template <class Tag>
struct Accessor {
   static typename Tag::type value;
};

template <class Tag>
typename Tag::type Accessor<Tag>::value;

template <class Tag, typename Tag::type p>
struct Initializer {
   Initializer() { Accessor<Tag>::value = p; }
   static Initializer instance;
};

template <class Tag, typename Tag::type p>
Initializer<Tag, p> Initializer<Tag, p>::instance;

} // namespace private_accessor_impl
} // namespace util
} // namespace nyaruga

#define NYARUGA_PRIVATE_ACCESSOR_INIT(tag, class_t, mem_t, mem_name) \
\
   namespace nyaruga::util::private_accessor_impl { \
\
   namespace user_defined_tags { \
\
   struct tag { \
      typedef mem_t class_t::*type; \
   }; \
   } \
\
   template struct nyaruga::util::private_accessor_impl::Initializer<user_defined_tags::tag, \
                                                                     &class_t::mem_name>; \
   }

#define NYARUGA_PRIVATE_ACCESSOR_INIT_FOR_STATIC(tag, class_t, mem_t, \
                                                 mem_name) \
\
   namespace nyaruga::util::private_accessor_impl { \
\
   namespace user_defined_tags { \
\
   struct tag { \
      typedef mem_t * type; \
   }; \
   } \
\
   template struct nyaruga::util::private_accessor_impl::Initializer<user_defined_tags::tag, \
                                                                     &class_t::mem_name>; \
   }

#define NYARUGA_PRIVATE_ACCESS(obj, tag) \
\
   obj.*nyaruga::util::private_accessor_impl:: \
           Accessor<nyaruga::util::private_accessor_impl::user_defined_tags::tag>::value

#define NYARUGA_PRIVATE_ACCESS_FOR_STATIC(tag) \
\
   *nyaruga::util::private_accessor_impl:: \
      Accessor<nyaruga::util::private_accessor_impl::user_defined_tags::tag>::value

/* how to use

#include "private_accessor.hpp"
#include <iostream>

// Target Class
struct A {
        explicit A(int mem1) : mem1(mem1) {}
        void print() {
                std::cout << "mem = " << mem1 << " smem = " << smem <<
std::endl;
        }
private:
        int mem1;
        static inline int smem = 999; // static
};

// 1. initialize accessor
// please use this macro in groval scope or namespace scope
// グローバルスコープまたは、名前空間内で使用してください
// 名前空間を使用するためです。現在のスコープは汚しません
NYARUGA_PRIVATE_ACCESSOR_INIT(A_mem1, A, int, mem1)  // arg :
任意のタグ、クラス型、メンバの型、メンバの名前
NYARUGA_PRIVATE_ACCESSOR_INIT_FOR_STATIC(A_smem, A, int, smem)  // static membar
ver

int main() {
        A a(1);

        // 2. access private member
        // accessor init が見えるところで使用してください
        std::cout << NYARUGA_PRIVATE_ACCESS_FOR_STATIC(A_smem) << std::endl; //
static
        std::cout << NYARUGA_PRIVATE_ACCESS(a, A_mem1) << std::endl;
        NYARUGA_PRIVATE_ACCESS_FOR_STATIC(A_smem) = 38; // static  arg: タグ名
        NYARUGA_PRIVATE_ACCESS(a, A_mem1) = 67;  // arg:
クラスオブジェクト、タグ名

        a.print();
}

//*/

#endif // #ifndef NYARUGA_UTIL_PRIVATE_ACCESSOR_HPP