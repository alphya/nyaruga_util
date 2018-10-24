#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>

namespace nyaruga_util
{

template<std::size_t sentinel, std::size_t count, typename R, typename F, typename Itr, typename Itr2>
std::function<R()> runtime_unpack_impl(F&& f, Itr&& current_itr, Itr2&& end_itr) noexcept
{
    if(current_itr == end_itr)
        return [f = std::forward<decltype(f)>(f)]{return f();};

    if constexpr(sentinel >= count)
    {
        return runtime_unpack_impl<sentinel, count+1, R>(
		    [car = *current_itr, f = std::forward<decltype(f)>(f)](auto&& ... args)
		    { return f(car, std::forward<decltype(args)>(args)...); }, 
			++current_itr, std::forward<decltype(end_itr)>(end_itr));
    }
    else
    {
        // std::cout << u8"引数が多すぎます。 " << sentinel << u8"個までの引数にしてください。\n";
        return [f = std::forward<decltype(f)>(f)]{return f();};
    }
}

template<std::size_t sentinel, typename F, template <typename...> typename T, typename U>
decltype(auto) runtime_unpack(F&& f, const T<U>& v) noexcept
{
    return runtime_unpack_impl<sentinel, 1, typename std::invoke_result_t<F, U>>
		(std::forward<decltype(f)>(f), v.begin(), v.end());
}

}  // namespace nyaruga_util

/*

int main() {
	using namespace nyaruga_util;

	std::vector<int> v{ 10, 20, 30 };

	const auto lambda = [](auto&& ... args)
	{
		return (args + ... + 0);
	};

	// 10 is max arg num
	std::cout << runtime_unpack<10>(lambda, v)(); // lambda(10, 20, 30)

}

*/