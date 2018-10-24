#pragma once

#include <type_traits>

namespace nyaruga_util
{

struct has_iterator_impl 
{
	template <class T>
	static std::true_type check(typename T::iterator*);

	template <class T>
	static std::false_type check(...);
};

struct has_iterator_2_impl 
{
	template <class T>
	static std::true_type check(typename T::iterator*);
};

template <class T>
class has_iterator :  // std::true_type or std::false_type
	public decltype(has_iterator_impl::check<T>(nullptr)) {};

template <class T>
class has_iterator_2 :  // itertorないとインスタンス化失敗
	public decltype(has_iterator_2_impl::check<T>(nullptr)) {};

}