
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

// ���̃N���X�́A
// https://theolizer.com/cpp-school2/cpp-school2-19/
// ���������Ă������̂ł�
// ���肪�Ƃ��������܂��B
// public �p���A�@private �R���X�g���N�^�A�@friend �w��@���Ďg��

template<class Derived>
class singleton : 
	// �R�s�[�^���[�u�֎~
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

// �e���v���[�g��ODR��O�ɂ��A������w�b�_�ɏ����Ă�OK
template<class Derived>
Derived& singleton<Derived>::m_instance = singleton<Derived>::get_instance();

}

using namespace singleton_impl_1;

/*

// �ʎY�V���O���g��A
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

// �ʎY�V���O���g��B
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

// static inline �ϐ���c++17�̋@�\�Ȃ̂ŁA
// c++11 �̊��̏ꍇ�A�K�Xstatic�ɓǂ݂����āA�N���X�O�ŏ��������Ă��������I
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
	~A(){ std::cout << "�R���X�g���N�^�̌Ă΂ꂽ��: " << i << "��" << "\n"; }
	std::string k;
	void a(int i) { std::cout << i  << "   " << k << "\n"; }
	static inline int i;
};

int main() {
	// �R���X�g���N�^��"hello"��n���Ă��܂��BA::A(std::string)
	A& x = singletonNS::singleton::get_singleton<A>("hello");
	x.a(1);  // 1   hello

	// �V���O���g���Ȃ̂ŁAmake_singleton������Ă�ł��A�����I�u�W�F�N�g���A���Ă���悤�ɂ��Ă܂�
	// ���̏ꍇ�ɁA�����͂̒l�͖������Ă܂��B�ł����A�����̌^�ɍ����R���X�g���N�^���K�v�ł�
	A& z = singletonNS::singleton::get_singleton<A>("world");
	z.a(2);  // 2   hello
	A& w = singletonNS::singleton::get_singleton<A>();  // A::A() ���K�v�ł�
	w.a(3);  // 3   hello

	// �L���Ȓl��ێ����Ă��邩�𔻒肵�܂�
	if (singletonNS::singleton::has_singleton<A>()) {

		// ����Ȍ`�ŃI�u�W�F�N�g�����炦�܂�
		// �I�u�W�F�N�g������Ă��Ȃ��̂ɂɌĂԂƁAstd::logic_error ��O�𓊂��܂�
		A& y = singletonNS::singleton::get<A>();
		y.a(4);  // 4   hello
	}

	// ��������i�ꉞ�X�}�[�g�|�C���^�g���Ă���̂ŁA�Ȃ��Ă����������H�j
	// ������鏇�����d�v�Ȏ��͂�����ƌĂ�ł��������I
	singletonNS::singleton::clear();
}

*/

} // namespace singleton_impl_2

using singleton_impl_2::get_as_singleton;

} // namespace nyaruga_util


#endif // #ifndef NYARUGA_UTIL_SINGLETON_HPP