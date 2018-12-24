
#include <iostream>
#include <functional>
#include <limits>
#include <utility>

//        Copyright Tomasz Kamiński 2013 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace type_traits
{
	template<typename T>
	struct target_type
	{
		typedef void type;
	};

	template<typename Class, typename Member>
	struct target_type<Member Class::*>
	{
		typedef Class type;
	};

	//Is reference to pointer target or derived
	template<typename Object, typename Pointer>
	struct is_target_reference :
		public std::integral_constant<
		bool,
		std::is_reference<Object>::value &&
		std::is_base_of<
		typename target_type<Pointer>::type,
		typename std::decay<Object>::type
		>::value
		>
	{};

	namespace detail
	{
		//MPL or
		constexpr bool predicate_or()
		{
			return false;
		}

		template<typename Pred, typename... Preds>
		constexpr bool predicate_or(Pred&& pred, Preds&&... preds)
		{
			return pred || predicate_or(preds...);
		}

		template<typename Object, typename TargetType>
		struct is_wrapper_compatible_with_member_pointer_impl
			: std::integral_constant<bool, predicate_or(
				std::is_convertible<Object, TargetType&>{},
				std::is_convertible<Object, TargetType const&>{},
				std::is_convertible<Object, TargetType&&>{},
				std::is_convertible<Object, TargetType const&&>{}) >
		{};

		template<typename Object>
		struct is_wrapper_compatible_with_member_pointer_impl<Object, void>
			: std::false_type
		{};
	}

	template<typename Object, typename Pointer>
	struct is_wrapper_compatible_with_member_pointer
		: detail::is_wrapper_compatible_with_member_pointer_impl<Object, typename target_type<Pointer>::type>
	{};

	namespace detail
	{
		template<typename Object, typename Pointer>
		constexpr bool is_pointer_compatible_with_member_pointer_impl(typename std::decay<decltype(*std::declval<Object>())>::type*)
		{
			return is_wrapper_compatible_with_member_pointer<decltype(*std::declval<Object>()), Pointer>::value;
		}

		template<typename Object, typename Pointer>
		constexpr bool is_pointer_compatible_with_member_pointer_impl(...)
		{
			return false;
		}
	}

	template<typename Object, typename Pointer>
	struct is_pointer_compatible_with_member_pointer
		: public std::integral_constant<bool, detail::is_pointer_compatible_with_member_pointer_impl<Object, Pointer>(0)>
	{};
}

#endif //TYPE_TRAITS_HPP


// 微分します 7点近似です

// メンバ関数用
template<typename Functor, typename Object>
constexpr auto diff(Functor&& functor, Object&& object, const double a)
->  typename std::enable_if<
	std::is_member_function_pointer<
	typename std::decay<Functor>::type
	>::value &&
	type_traits::is_target_reference<
	Object&&,
	typename std::decay<Functor>::type
	>::value,
	decltype((std::forward<Object>(object).*functor)(a))
>::type
{
	constexpr auto h = 1e-7;
	return ((object.*functor)(a + 3 * h) - 9 * (object.*functor)(a + 2 * h) 
		+ 45 * (object.*functor)(a + h) - 45 * (object.*functor)(a - h)
		+ 9 * (object.*functor)(a - 2 * h) - (object.*functor)(a - 3 * h)) / (60 * h); 
}

// なんだろう
template<typename Functor, typename Object>
constexpr auto diff(Functor&& functor, Object&& object, const double a)
->  typename std::enable_if<
	std::is_member_function_pointer<
	typename std::decay<Functor>::type
	>::value &&
	!type_traits::is_target_reference<
	Object&&,
	typename std::decay<Functor>::type
	>::value,
	decltype(((*std::forward<Object>(object)).*functor)(a))
>::type
{
	constexpr auto h = 1e-7;
	return (((*object).*functor)(a + 3 * h) - 9 * ((*object).*functor)(a + 2 * h) 
		+ 45 * ((*object).*functor)(a + h) - 45 * ((*object).*functor)(a - h)
		+ 9 * ((*object).*functor)(a - 2 * h) - ((*object).*functor)(a - 3 * h)) / (60 * h); 
}

// これを呼んだところで、何をすればいいだろうか
template<typename Functor, typename Object>
constexpr auto diff(Functor&& functor, Object&& object)
->  typename std::enable_if<
	std::is_member_object_pointer<
	typename std::decay<Functor>::type
	>::value &&
	type_traits::is_target_reference<
	Object&&,
	typename std::decay<Functor>::type
	>::value,
	decltype((std::forward<Object>(object).*functor))
>::type
{
	return std::forward<Object>(object).*functor;
}

// これも...
template<typename Functor, typename Object>
constexpr auto diff(Functor&& functor, Object&& object)
->  typename std::enable_if <
	std::is_member_object_pointer<
	typename std::decay<Functor>::type
	>::value &&
	!type_traits::is_target_reference<
	Object&&,
	typename std::decay<Functor>::type
	>::value,
	decltype((*std::forward<Object>(object)).*functor)
>::type
{
	return (*std::forward<Object>(object)).*functor;
}

//通常関数、スタティックメンバ関数用
template<typename Functor>
constexpr auto diff(Functor&& functor, const double a)
->  typename std::enable_if<
	!std::is_member_pointer<
	typename std::decay<Functor>::type
	>::value,
	decltype(std::forward<Functor>(functor)(a))
>::type
{
	    constexpr auto h = 1e-7;
		return (functor(a + 3 * h) - 9 * functor(a + 2 * h) + 45 * functor(a + h) 
			- 45 * functor(a - h) + 9 * functor(a - 2 * h) - functor(a - 3 * h)) / (60 * h);
}

constexpr auto f(double x)
{
	return 300 * x + 600;
};

struct s
{
	constexpr s() = default;
	constexpr decltype(auto) f(double x)
	{
		return x * x * x;
	}
	static constexpr decltype(auto) s_f(double x)
	{
		return 15 * x * x;
	}
};

/* output
auto main() -> int
{
	[[maybe_unused]] constexpr auto f_ = [](double x)
	{
		return - 30000 / x;
	};

	[[maybe_unused]] s ss;

	constexpr auto result_lambda = diff(f_, 10);
	constexpr auto result_func = diff(f, 10);
	constexpr auto result_member_func = diff(&s::f, ss, 10);
	constexpr auto result_static_member_func = diff(&s::s_f, 10);

	std::cout << "理論値: 300" << "\n"
		<< result_lambda << "\n" 
		<< result_func << "\n"
		<< result_member_func << "\n"
		<< result_static_member_func << "\n";
}

理論値: 300
300
300
300
300
*/
