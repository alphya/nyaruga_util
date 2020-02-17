//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// この時間計測クラスは、
// C++ 処理時間計測classを作ってみる - Qiita
// https://qiita.com/tyanmahou/items/8497d6e815ebf7ea90c6
// よりコピペしたものです

#ifndef NYARUGA_UTIL_CHECK_TIME_HPP
#define NYARUGA_UTIL_CHECK_TIME_HPP

#pragma once

#include <chrono>
#include <string>

namespace nyaruga::util {

namespace detail
{
    template<class T>
    struct is_duration :std::false_type
    {};
    template<class Rep, class Period>
    struct is_duration<std::chrono::duration<Rep, Period>> :std::true_type
    {};

    ///<summary>
    ///処理計測実装
    ///</summary>
    template<class Duration>
    class ProcessingTime_impl
    {
        static_assert(is_duration<Duration>::value, "tempate parameter requires std::chrono::duration");

    public:
        ProcessingTime_impl(const std::string& name = "Process", bool start = true) :
            m_name(name),
            m_isActive(start)
        {
            if (start)
            {
                this->restart();
            }
        }
        ~ProcessingTime_impl()
        {
            this->stop();
        }

        ///<summary>
        ///計測のリスタート
        ///</summary>
        void restart()&
        {
            m_start = std::chrono::system_clock::now();
            m_isActive = true;
        }
        ///<summary>
        ///計測を終了し出力
        ///</summary>
        void stop()&
        {
            if (!m_isActive)
                return;
            const auto end = std::chrono::system_clock::now();
            const auto elapsed = std::chrono::duration_cast<Duration>(end - m_start).count();
            std::cout << m_name << " : " << elapsed << " " << this->getUnit() << std::endl;

            m_isActive = false;
        }
    private:

        std::string getUnit()const
        {
            if constexpr(std::is_same_v<Duration, std::chrono::nanoseconds>)return "ns";
            if constexpr(std::is_same_v<Duration, std::chrono::microseconds>)return "us";
            if constexpr(std::is_same_v<Duration, std::chrono::milliseconds>)return "ms";
            if constexpr(std::is_same_v<Duration, std::chrono::seconds>)return "s";
            if constexpr(std::is_same_v<Duration, std::chrono::minutes>)return "min";
            if constexpr(std::is_same_v<Duration, std::chrono::hours>)return "h";

            return " ";
        }
        std::string m_name;
        std::chrono::system_clock::time_point m_start;
        bool m_isActive;

    };

}

///<summary>
///処理時間の計測
///</summary>
///<param name="name">
///処理名
///</param>
///<param name="start">
///すぐに計測を開始するか? true=>開始
///</param>
template<
    class Duration = std::chrono::milliseconds,
    std::enable_if_t<detail::is_duration<Duration>::value>* = nullptr
>
[[nodiscard]] detail::ProcessingTime_impl<Duration> check_time(const std::string& name = "Process", bool start = true)
{
    return detail::ProcessingTime_impl<Duration>(name, start);
}

/* 使い方
int main()
{
    {
        auto p = check_time<std::chrono::nanoseconds>("nanosec");

        /*
        処理
        */

    }
    {
        auto p = check_time("millisec");

        /*
        処理
        */

    }
    return 0;
}
*/

} // nyaruga :: util

#endif // NYARUGA_UTIL_CHECK_TIME_HPP
