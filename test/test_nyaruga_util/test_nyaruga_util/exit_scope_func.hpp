
#include <functional>
#include <type_traits>

namespace
{
    struct exit_scope
	{
		constexpr exit_scope() noexcept = delete;
		template<typename T>
		constexpr exit_scope(T && f) noexcept : func(std::move(f)) {}
		~exit_scope() noexcept { func(); }
	private: std::function<void()> func;
	};
}

/*
使い方

// 任意のスコープ
exit_scope scope_end([/*任意のキャプチャ*/]()
{
   // 任意の処理 
});

*/

/*
なんの役に立つか

この構造体のオブジェクトのデストラクタが呼ばれるときに実行される関数を
指定できる。解放忘れ、例外安全などの役に立つ。

*/