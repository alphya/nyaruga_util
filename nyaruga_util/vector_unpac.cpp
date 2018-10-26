#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>

template<std::size_t sentinel, std::size_t count, typename R, typename F, typename Itr, typename Itr2>
std::function<R()> runtime_unpack_impl(F&& f, Itr && current_itr, Itr2 && end_itr)
{
    if(current_itr == end_itr)
        return [f = std::forward<F>(f)](){return f();};

    if constexpr(sentinel >= count)
    {
        auto f_ = [car = current_itr, f = std::forward<F>(f)](auto&& ... args)
        {
            return f(*car, std::forward<decltype(args)>(args)...);
        };
        return runtime_unpack_impl<sentinel, count+1, R>
            (f_, std::forward<decltype(current_itr)>(++current_itr), 
             std::forward<decltype(end_itr)>(end_itr));
    }
    else
    {
        std::cout << u8"引数が多すぎます。 " << sentinel << u8"個までの引数にしてください。\n";
        return [f = std::forward<F>(f)](){return f();};
    }
}

template<std::size_t sentinel, typename F, typename T>
auto runtime_unpack(F&& func, const T& container) -> std::function<decltype(func(*std::begin(container)))()>
{
    return runtime_unpack_impl<sentinel, 1, decltype(func(*std::begin(container)))>(std::forward<F>(func), std::begin(container), std::end(container));
}

/*

int main()
{
    std::vector<int> v{10, 20, 30};

    const auto lambda = [](auto && ... args)
    {
        return (0 + ... + std::forward<decltype(args)>(args));
    };

    std::cout << runtime_unpack<10>(lambda, v)();
}

*/
