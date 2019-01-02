
//              Copyright (c) 2018 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



#ifndef NYARUGA_UTIL_SINGLETON_HPP
#define NYARUGA_UTIL_SINGLETON_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/noncopyable.hpp>
#include <nyaruga_util/nonmovable.hpp>
#include <boost/polymorphic_cast.hpp>
#include <mutex>
#include <utility>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
#include <exception>

namespace nyaruga_util {

namespace singleton_impl_1 { // protection from unintended ADL

// このクラスは、
// https://theolizer.com/cpp-school2/cpp-school2-19/
// からもらってきたものです
// ありがとうございます。
// public 継承、　private コンストラクタ、　friend 指定　して使う

template<class Derived>
class singleton : 
	// コピー／ムーブ禁止
	private boost::noncopyable, private nyaruga_util::nonmovable
{
	static Derived& m_instance ;
	static void use(Derived const&) {}
protected:
	singleton() = default;
public:
	static Derived& get_instance()
	{
		static Derived instance;
		use(m_instance);
		return instance;
	}
};

// テンプレートのODR例外により、これをヘッダに書いてもOK
template<class Derived>
Derived& singleton<Derived>::m_instance = singleton<Derived>::get_instance();

}

using namespace singleton_impl_1;

/*

// 量産シングルトンA
class MassSingletonA : public singleton<MassSingletonA>
{
	friend class singleton<MassSingletonA>;
	int     mData;
	MassSingletonA() : mData(12345) { }
public:
	void print(char const* iName)
	{
		std::cout << iName << ".print() mData = " << mData << " (" << &mData << ")\n";
	}
};

// 量産シングルトンB
class MassSingletonB : public singleton<MassSingletonB>
{
	friend class singleton<MassSingletonB>;
	std::string mData;
	MassSingletonB() : mData("default") { }
public:
	void print(char const* iName)
	{
		std::cout << iName << ".print() mData = " << mData << " (" << &mData << ")\n";
	}
};

int main()
{
	std::cout << "start of main()\n";

	auto& aMassSingletonA0 = MassSingletonA::get_instance();
	aMassSingletonA0.print("aMassSingletonA0");
	auto& aMassSingletonA1 = MassSingletonA::get_instance();
	aMassSingletonA1.print("aMassSingletonA1");

	auto& aMassSingletonB0 = MassSingletonB::get_instance();
	aMassSingletonB0.print("aMassSingletonB0");
	auto& aMassSingletonB1 = MassSingletonB::get_instance();
	aMassSingletonB1.print("aMassSingletonB1");

	std::cout << "end of main()\n";
}

// output
MassSingletonA()
MassSingletonB()
start of main()
aMassSingletonA0.print() mData = 12345 (0x6026ec)
aMassSingletonA1.print() mData = 12345 (0x6026ec)
aMassSingletonB0.print() mData = default (0x602700)
aMassSingletonB1.print() mData = default (0x602700)
end of main()
~MassSingletonB()
~MassSingletonA()

*/
namespace singleton_impl_2 { // protection from unintended ADL

// static inline 変数はc++17の機能なので、
// c++11 の環境の場合、適宜staticに読みかえて、クラス外で初期化してください！
namespace impl
{

class singleton_finalizer final {
	singleton_finalizer() = delete;
	static inline std::vector<std::function<void()>> finalizers;
public:
	static void add_finalizer(std::function<void()>&& func) {
		finalizers.emplace_back(std::forward<decltype(func)>(func));
	}
	static void clear() noexcept {
		std::for_each(finalizers.crbegin(), finalizers.crend(), [](auto&& fn) {
			fn();
			}), finalizers.clear();
	}
};

template <typename T>
class singleton_impl final {
	singleton_impl() = delete;
	static inline std::once_flag init_flag;
	static inline std::unique_ptr<T> instance;
	template <typename ... Args>
	static void get_singleton_impl(Args&& ... args) {
    	instance = std::make_unique<T>(std::forward<Args>(args)...);
	    singleton_finalizer::add_finalizer([] {instance.reset(nullptr); });
	}
public:
	template <typename ... Args>
	static T& get_singleton(Args&& ... args) {
		std::call_once(init_flag, get_singleton_impl<Args...>, std::forward<Args>(args)...);
		return *instance.get();
	}
	static T* get() {
		return[ptr = instance.get()]
		{ return ptr ? ptr : throw std::logic_error("This type had not singleton object."); }();
	}
	static bool has_singleton() { return instance.get(); }
};

}  // namespace impl

class get_as_singleton final {
	get_as_singleton() = delete;
public:
	template<typename T, typename ... Args>
	static T& get_singleton(Args&& ... args) {
		return impl::singleton_impl<T>::get_singleton(std::forward<Args>(args)...);
	}
	template <typename T>
	static T& get() { return *impl::singleton_impl<T>::get(); }
	template <typename T>
	static bool has_singleton() { return impl::singleton_impl<T>::has_singleton(); }
	static void clear() noexcept { impl::singleton_finalizer::clear(); }
};

/*
#include <iostream>
#include <string>

class A {
public:
	A(){ ++i; };
	explicit A(std::string j) : k(j) { ++i; }
	~A(){ std::cout << "コンストラクタの呼ばれた回数: " << i << "回" << "\n"; }
	std::string k;
	void a(int i) { std::cout << i  << "   " << k << "\n"; }
	static inline int i;
};

int main() {
	// コンストラクタに"hello"を渡しています。A::A(std::string)
	A& x = singletonNS::singleton::get_singleton<A>("hello");
	x.a(1);  // 1   hello

	// シングルトンなので、make_singletonを何回呼んでも、同じオブジェクトが帰ってくるようにしてます
	// この場合に、引数はの値は無視してます。ですが、引数の型に合うコンストラクタが必要です
	A& z = singletonNS::singleton::get_singleton<A>("world");
	z.a(2);  // 2   hello
	A& w = singletonNS::singleton::get_singleton<A>();  // A::A() が必要です
	w.a(3);  // 3   hello

	// 有効な値を保持しているかを判定します
	if (singletonNS::singleton::has_singleton<A>()) {

		// こんな形でオブジェクトをもらえます
		// オブジェクトを作っていないのにに呼ぶと、std::logic_error 例外を投げます
		A& y = singletonNS::singleton::get<A>();
		y.a(4);  // 4   hello
	}

	// 解放処理（一応スマートポインタ使っているので、なくてもいいかも？）
	// 解放する順序が重要な時はきちんと呼んでください！
	singletonNS::singleton::clear();
}

*/

} // namespace singleton_impl_2

using singleton_impl_2::get_as_singleton;

} // namespace nyaruga_util


#endif // #ifndef NYARUGA_UTIL_SINGLETON_HPP