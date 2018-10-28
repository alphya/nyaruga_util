#include <iostream>

#include "../../../nyaruga_util/diff.hpp"



#include <functional>
#include <type_traits>
#include "../../../nyaruga_util/false_v.hpp"

namespace nyaruga_util {


template<std::size_t current_pos, std::size_t arg_pos, typename F, typename Arg>
decltype(auto) bind_any_pos_arg_impl(F && f, Arg && arg)
{
	if constexpr (current_pos < arg_pos)
	{
		return bind_any_pos_arg_impl<current_pos + 1, arg_pos>(
			[f = std::forward<F>(f)](auto && ... args)
		{ return std::bind(f, std::placeholders::_1, args...)(); }, arg);
	}
	else if constexpr (current_pos == arg_pos)
	{
		return [f = std::forward<F>(f), arg = std::forward<Arg>(arg)]
		(auto && ... args) { return std::bind(f, arg, args...)(); };
	}
	else
		static_assert(false_v<F>, "arg_pos is larger than f arg num");
}

// w’è‚µ‚½ˆø”‚Ì‚İ‚ğ“K—p‚µ‚½ŠÖ”‚ğ•Ô‚·‚ŠKŠÖ” f(arg_pos = 1, 2, 3, 4...)
template<std::size_t arg_pos, typename F, typename Arg>
decltype(auto) bind_any_pos_arg(F && f, Arg && arg)
{
	return bind_any_pos_arg_impl<1, arg_pos>(std::forward<F>(f), std::forward<Arg>(arg));
}

}  // nyaruga_util



int main()
{

	auto fn = [](int i, double d, float f, int h)
	{
		std::cout << i << " " << d << " " << f;
	};

	// ‘æ1ˆø”‚Ì‚İ‚ğæ‚É“n‚·
	using namespace std::placeholders;
	auto ninjin = std::bind(fn, _1, 45, _2);
	auto Pike = std::bind(ninjin, _1, 99.f);
	//Pike(123, 8);

	auto && aaa = nyaruga_util::bind_any_pos_arg<2>(fn, 47);

	aaa(1, 2, 3);

}










