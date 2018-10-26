#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>

template<std::size_t sentinel, std::size_t count, typename R, 
typename F, typename Itr>
std::function<R()> runtime_unpack_impl(F&& f, Itr && current_itr, Itr && end_itr)
{
    if(current_itr == end_itr)
        return [f = std::forward<F>(f)]{return f();};

    if constexpr(sentinel >= count)
    {
        return runtime_unpack_impl<sentinel, count+1, R>(
        [car = *current_itr++, f = std::forward<F>(f)](auto&& ... args)
        {
            return f(car, std::forward<decltype(args)>(args)...);
        }, std::forward<Itr>(current_itr), std::forward<Itr>(end_itr));
    }
    else
    {
        std::cout << u8"引数が多すぎます。 " << sentinel << u8"個までの引数にしてください。\n";
        return [f = std::forward<F>(f)]{return f();};
    }
}

template<std::size_t sentinel, typename F, typename Container>
auto runtime_unpack(F&& f, const Container& v) -> decltype(std::enable_if_t<std::is_invocable_v<F, decltype(*std::begin(v))>>(), std::function<decltype(f(*std::begin(v)))()>())
{
    return runtime_unpack_impl<sentinel, 1, decltype(f(*std::begin(v)))>
    (std::forward<F>(f), std::begin(v), std::end(v));
}

namespace 
{
    template <typename ... T>
    constexpr bool false_v = false;
}

template<std::size_t sentinel, typename F, typename Container>
auto runtime_unpack(F , const Container& v) -> decltype(std::enable_if_t<!std::is_invocable_v<F, decltype(*std::begin(v))>>(), std::function<void()>())
{
    static_assert(false_v<F>, \
"\n\n===============================================================\n\
runtime_unpack (F && f, const Container & v) error\n\
===============================================================\n\
This function requires a function such that \n\
std::is_invocable_v<F, decltype(*std::begin(v))>> is true!\n\
If you use the function fn(...) and use the container std::vector v[3],\n\
and If you code like runtime_unpac<10>(fn, v), \n\
This function unpac a function like fn (v[0], v[1], v[2]).\n\
===============================================================\n\n");
}

int main()
{
    std::vector<int> v{10, 20, 30};

    const auto lambda = [](char**, auto && ... args)
    {
        return (0 + ... + std::forward<decltype(args)>(args));
    };

    std::cout << runtime_unpack<10>(lambda, v)();
}
