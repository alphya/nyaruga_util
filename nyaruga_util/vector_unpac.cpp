#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>

template<std::size_t sentinel, std::size_t count, typename F, typename T, typename Alloc>
std::function<void()> runtime_unpack_impl(F&& f, const std::vector<T, Alloc>& v)
{
    if(v.empty())
        return std::function<void()>([f = std::forward<F>(f)](){return f();});

    if constexpr(sentinel >= count)
    {
        auto cdr = v;
        cdr.erase(cdr.begin());
        auto f_ = [car = v.front(), f = std::forward<F>(f)](auto&& ... args)
        {
            f(car, std::forward<decltype(args)>(args)...);
        };
        return runtime_unpack_impl<sentinel, count+1>(f_, cdr);
    }
    else
    {
        std::cout << u8"引数が多すぎます。 " << sentinel << u8"個までの引数にしてください。\n";
        return std::function<void()>([f = std::forward<F>(f)](){return f();});
    }
}

template<std::size_t sentinel, typename F, typename T, typename Alloc>
std::function<void()> runtime_unpack(F&& f, const std::vector<T, Alloc>& v)
{
    return runtime_unpack_impl<sentinel, 1>(std::forward<F>(f), v);
}

int main()
{
    std::vector<int> v{10, 20, 30};

    const auto lambda = [](auto&& ... args)
    {
        (std::cout << ... << std::forward<decltype(args)>(args)) << '\n';
    };

    runtime_unpack<10>(lambda, v)();
}