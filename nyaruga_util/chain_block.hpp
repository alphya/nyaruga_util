//              Copyright (c) 2020 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NYARUGA_UTIL_CHAIN_BLOCK_HPP
#define NYARUGA_UTIL_CHAIN_BLOCK_HPP

#pragma once

#include <tuple>
#include <compare>
#include <utility>
#include <concepts>
#include <nyaruga_util/category.hpp>

// 引数を渡したり、受け取ったりできるブロックのような機能を持つモナド（あやしい）です
// 見ることができるローカル変数を制限したりしたいときに便利です

namespace nyaruga::util {

namespace chain_block_ {

inline struct exit_chain_t{} exit_chain;
inline struct begin_chain_t{} begin_chain;

template <typename ... Types>
struct pass_param : public std::tuple<Types...> { using std::tuple<Types...>::tuple; };

template <typename... Types>
pass_param(Types...)->pass_param<Types...>;

namespace detail
{
   template <typename T>
   constexpr auto is_pass_param_impl(T) -> std::false_type;

   template <template<class...> class T, typename ... Args>
      requires std::convertible_to<T<Args...>, pass_param<Args...>>
   constexpr auto is_pass_param_impl(T<Args...>) -> std::true_type;

   template <typename T>
   concept is_pass_param = decltype(is_pass_param_impl(std::declval<T>()))::value;

   template <typename T>
   concept is_begin_chain = std::convertible_to<T, begin_chain_t>;

   template <typename T>
   concept is_exit_chain = std::convertible_to<T, exit_chain_t>;
}

template <class T>
class chain_block {
private:
   T m_val;
   bool m_exit_flag;
public:
   constexpr chain_block() noexcept : m_exit_flag(false) {}
   constexpr chain_block(const exit_chain_t&) noexcept : m_exit_flag(true) {}
   constexpr chain_block(const T & x) noexcept : m_val(x), m_exit_flag(false) {}
   constexpr chain_block(T && x) noexcept : m_val(std::forward<T>(x)), m_exit_flag(false) {}
   template <std::convertible_to<T> U>
      requires std::is_copy_assignable_v<T>
   constexpr auto& operator = (const chain_block<U>& other) noexcept(std::is_nothrow_assignable_v<T>) { T tmp = other.m_val; swap(m_val, tmp); return *this; }
   template <std::convertible_to<T> U>
      requires std::is_move_assignable_v<T>
   constexpr auto& operator = (chain_block<U>&& other) noexcept(std::is_nothrow_move_assignable_v<T>) { m_val = std::move(other.m_val); return *this; }
   constexpr auto& unwrap() noexcept { return m_val; }
   constexpr auto exit_flag() { return m_exit_flag; }
   // constexpr auto operator <=> (const chain_block&) const noexcept = default;
};

template <class T>
   requires detail::is_exit_chain<T>
chain_block(T)->chain_block<std::nullptr_t>;

template <class T, class F>
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(f(x.unwrap())))
   -> chain_block<category::apply_mu<chain_block, decltype(f(x.unwrap()))>>
   requires category::kleisli_morphism_from<F, chain_block, T>
{
   return !x.exit_flag() ? f(x.unwrap()) : chain_block<category::apply_mu<chain_block, decltype(f(x.unwrap()))>>(exit_chain);
}

template <class T, class F>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(f(std::move(x.unwrap()))))
   -> chain_block<category::apply_mu<chain_block, decltype(f(std::move(x.unwrap())))>>
   requires category::kleisli_morphism_from<F, chain_block, T>
{
   return (!x.exit_flag()) ? f(std::move(x.unwrap())) : chain_block<category::apply_mu<chain_block, decltype(f(std::move(x.unwrap())))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_pass_param<T>
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(std::apply(f, x.unwrap())))
   -> chain_block<category::apply_mu<chain_block, decltype(std::apply(f, x.unwrap()))>>
{
   return !x.exit_flag() ? std::apply(f, x.unwrap()) : chain_block<category::apply_mu<chain_block, decltype(std::apply(f, x.unwrap()))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_pass_param<T>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(std::apply(f, std::move(x.unwrap()))))
   -> chain_block<category::apply_mu<chain_block, decltype(std::apply(f, std::move(x.unwrap())))>>
{
   return (!x.exit_flag()) ? std::apply(f, std::move(x.unwrap())) : chain_block<category::apply_mu<chain_block, decltype(std::apply(f, std::move(x.unwrap())))>>(exit_chain);
}

template <class T, class F>
   requires detail::is_begin_chain<T> 
constexpr auto operator>=(const chain_block<T> & x, F && f) noexcept(noexcept(f()))
   -> chain_block<category::apply_mu<chain_block, decltype(f())>>
{
   return f();
}

template <class T, class F>
   requires detail::is_begin_chain<T>
constexpr auto operator>=(chain_block<T> && x, F && f) noexcept(noexcept(f()))
   -> chain_block<category::apply_mu<chain_block, decltype(f())>>
{
   return f();
}

template <class T>
constexpr auto ret(T&& x) noexcept -> decltype(chain_block(std::forward<T>(x)))
{
   return chain_block(std::forward<T>(x));
}

template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, chain_block>
constexpr auto operator>(const chain_block<T>& x, Func && f) noexcept(noexcept(f(x.unwrap())))
{
   return !x.exit_flag() ? chain_block<category::apply_morphism<T, Func>>{ f(x.unwrap()) } : chain_block<category::apply_morphism<T, Func>>{ exit_chain };
}
   
template <typename T, typename Func>
   requires category::morphism_from<Func, T> && category::kleisli_object<category::apply_morphism<T, Func>, chain_block>
constexpr auto operator>(chain_block<T>&& x, Func && f) noexcept(noexcept(f(std::move(x.unwrap()))))
{
   return !x.exit_flag() ? chain_block<category::apply_morphism<T, Func>>{ f(std::move(x.unwrap())) } : chain_block<category::apply_morphism<T, Func>>{ exit_chain };
}

template <typename T, typename Func>
   requires detail::is_pass_param<T> && category::kleisli_object<decltype(std::apply(std::declval<Func>(), std::declval<T>())), chain_block>
constexpr auto operator>(const chain_block<T>& x, Func && f) noexcept(noexcept(std::apply(f, x.unwrap())))
{
   return !x.exit_flag() ? chain_block<decltype(std::apply(f, x.unwrap()))>{ std::apply(f, x.unwrap()) } : chain_block<decltype(std::apply(f, x.unwrap()))>{ exit_chain };
}
   
template <typename T, typename Func>
   requires detail::is_pass_param<T> &&category::kleisli_object<decltype(std::apply(std::declval<Func>(), std::declval<T>())), chain_block>
constexpr auto operator>(chain_block<T>&& x, Func && f) noexcept(noexcept(std::apply(f, std::move(x.unwrap()))))
{
   return !x.exit_flag() ? chain_block<decltype(std::apply(f, std::move(x.unwrap())))>{ std::apply(f, std::move(x.unwrap())) }
                       : chain_block<decltype(std::apply(f, std::move(x.unwrap())))>{ exit_chain };
}

template <typename T, typename Func>
   requires detail::is_begin_chain<T> && category::kleisli_object<decltype(f()), chain_block> 
constexpr auto operator>(const chain_block<T> & x, Func && f) noexcept(noexcept(f()))
{
   return chain_block<decltype(f())>{ f() };
}

template <typename T, typename Func>
   requires detail::is_begin_chain<T> && category::kleisli_object<decltype(f()), chain_block> 
constexpr auto operator>(chain_block<T> && x, Func && f) noexcept(noexcept(f()))
{
   return chain_block<decltype(f())>{ f() };
}


} // namespace chain_block_

using namespace chain_block_;

} // nyaruga :: util

template <class... Types>
class std::tuple_size<nyaruga::util::pass_param<Types...>>
   : public std::integral_constant<std::size_t, sizeof...(Types)> {
};

/* 使用例

#include <algorithm>
#include <boost/format.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <nyaruga_util/chain_block.hpp>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
   using namespace nyaruga::util;
   using namespace std;

   chain_block{begin_chain} >= // このプログラム全体

      []() { // カレントディレクトリの CMakeLists.txt を開く（もしあれば）

         auto ifs = ifstream(filesystem::current_path() /= "CMakeLists.txt");
         return ifs ? pass_param(move(ifs)) : chain_block<decltype(pass_param(move(ifs)))>(exit_chain);

      } > [](ifstream&& ifs) { // CMakeLists.txt から test のディレクトリ名を抽出する
      
         // std::string にファイルの内容を渡す
         string file_contents{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};

         // subdirs(...) にマッチする
         smatch subdirs_etc;
         regex_search(file_contents, subdirs_etc, regex(R"(subdirs\(([\s\S]*)\))"));

         // ... の部分から、テストがあるディレクトリ名を抽出する
         string subdirs = subdirs_etc[1].str();
         regex re = regex(R"(\S+)");
         sregex_iterator end, it{ subdirs.begin(), subdirs.end(), re };

         set<string> test_dir_names;
         for (; it != end; ++it)
            test_dir_names.insert((*it).str());

         return pass_param{ move(file_contents), move(test_dir_names) };

      } >= [](string&& file_contents, set<string>&& test_dir_names) {
         // 現在のサブディレクトリのうち、新しく追加されたものを CMakeLists.txt の subdirs に追加し、
         // そのディレクトリの中に CMakeLists.txt を追加する

            // 現在あるディレクトリ名
         set<string> current_dirs = move([] {
            set<string> dirs;
            for (const auto& it : filesystem::directory_iterator(filesystem::current_path()))
               if (it.is_directory()) dirs.insert(it.path().filename().string());
            return dirs;
            }());

         // 新しくできたディレクトリ名（CMakeLists.txt の subdir にないもの）
         set<string> new_dirs;
         set_difference(current_dirs.begin(), current_dirs.end(),
            test_dir_names.begin(), test_dir_names.end(),
            inserter(new_dirs, new_dirs.end()));

         // 現在のサブディレクトリのうち、新しく追加されたものを CMakeLists.txt の subdirs に追加する
         using namespace std::string_literals;
         string new_subdir_txt = "subdirs(\n"s +
            [&current_dirs]{
               string result;
               for (const auto& it : current_dirs)
                  result += "    " + it + "\n";
               return result;
         }() + ")";

         string new_cmakelists_text =
            regex_replace(file_contents, regex(R"(subdirs\([\s\S]*\))"), new_subdir_txt);

         ofstream ofs(filesystem::current_path() /= "CMakeLists.txt", ios::trunc);
         ofs << new_cmakelists_text;
         ofs.close();

         // 新しくできたディレクトリ中の .cpp ファイル名をテストの名前として抽出する
         unordered_map<string, string> test_case_names; // <ディレクトリ名, テスト名>
         for (const auto& dir_name : new_dirs)
            for (const auto& it : filesystem::directory_iterator(filesystem::current_path() /= dir_name))
               if (it.path().extension() == ".cpp") test_case_names[dir_name] = it.path().filename().replace_extension("").string();

         // 新しくできたディレクトリ中に CMakeLists.txt を作成する（もしなければ）
         ifstream ifs{ filesystem::current_path() /= "CMakeLists_template.txt" };
         string file_templates{ istreambuf_iterator<char>(ifs), istreambuf_iterator<char>() };
         for (const auto& dir_name : new_dirs)
            if (not filesystem::exists((filesystem::current_path() /= dir_name) /= "CMakeLists.txt"s) 
               && (test_case_names.find(dir_name) != test_case_names.end()))
                  ofstream{ (filesystem::current_path() /= dir_name) /= "CMakeLists.txt"s } 
                     << (boost::format(file_templates) % test_case_names[dir_name] % dir_name);
      
         return chain_block{exit_chain};
      };
}

*/

#endif // NYARUGA_UTIL_CHAIN_BLOCK_HPP
