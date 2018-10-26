#pragma once

#include <type_traits>

namespace nyaruga_util
{

	template <class, class = void>
	struct has_iterator
		: std::false_type {};

	template <class T>
	struct has_iterator<T, std::void_t<typename T::iterator>>
		: std::true_type {};

}

/*

#include <vector>
int main()
{
	static_assert(
		has_iterator<std::vector<int>>::value,
		"vector‚ÍiteratorŒ^‚ğ‚Á‚Ä‚¢‚é");

	static_assert(
		!has_iterator<int>::value,
		"int‚ÍiteratorŒ^‚ğ‚Á‚Ä‚¢‚È‚¢");
}

*/