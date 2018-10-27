#pragma once

// 元となるコード
// https://gist.github.com/1528856
// Accessing Private Data 
// c.f. http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
// c.f. http://d.hatena.ne.jp/redboltz/touch/20120111/1326292284

namespace nyaruga_util {
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

}  // private_accessor_impl
}  // nyaruga_util

#define NYARUGA_PRIVATE_ACCESSOR_INIT(tag, class_t, mem_t, mem_name) \
namespace nyaruga_util::private_accessor_impl{\
struct tag { typedef mem_t class_t::* type; };\
template struct nyaruga_util::private_accessor_impl::Initializer<tag, &class_t::mem_name>;\
}

#define NYARUGA_PRIVATE_ACCESSOR_INIT_FOR_STATIC(tag, class_t, mem_t, mem_name) \
namespace nyaruga_util::private_accessor_impl{\
struct tag { typedef mem_t * type; };\
template struct nyaruga_util::private_accessor_impl::Initializer<tag, &class_t::mem_name>;\
}

#define NYARUGA_PRIVATE_ACCESS(obj, tag) obj.*nyaruga_util::private_accessor_impl::\
Accessor<nyaruga_util::private_accessor_impl::tag>::value

#define NYARUGA_PRIVATE_ACCESS_FOR_STATIC(tag) *nyaruga_util::private_accessor_impl::\
Accessor<nyaruga_util::private_accessor_impl::tag>::value


/* how to use 

#include <iostream>
#include "private_accessor.hpp"

// Target Class
struct A {
	explicit A(int mem1) : mem1(mem1) {}
	void print() {
		std::cout << "mem = " << mem1 << " smem = " << smem << std::endl;
	}
private:
	int mem1;
	static inline int smem = 999; // static
};

// 1. initialize accessor
NYARUGA_PRIVATE_ACCESSOR_INIT(A_mem1, A, int, mem1)  // arg : 任意のタグ、クラス型、メンバの型、メンバの名前
NYARUGA_PRIVATE_ACCESSOR_INIT_FOR_STATIC(A_smem, A, int, smem)  // static membar ver

int main() {
	A a(1);

	// 2. access private member
	std::cout << NYARUGA_PRIVATE_ACCESS_FOR_STATIC(A_smem) << std::endl; // static
	std::cout << NYARUGA_PRIVATE_ACCESS(a, A_mem1) << std::endl;
	NYARUGA_PRIVATE_ACCESS_FOR_STATIC(A_smem) = 38; // static  arg: タグ名
	NYARUGA_PRIVATE_ACCESS(a, A_mem1) = 67;  // arg: クラスオブジェクト、タグ名

	a.print();
}

//*/