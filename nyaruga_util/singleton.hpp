
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

namespace nyaruga_util {

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

} // namespace nyaruga_util

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

#endif // #ifndef NYARUGA_UTIL_SINGLETON_HPP