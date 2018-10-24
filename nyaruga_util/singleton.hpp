#pragma once
// hpp

#include <mutex>
#include <utility>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
#include <exception>

namespace nyaruga_util {
	namespace singleton_implNS {

		// static inline �ϐ���c++17�̋@�\�Ȃ̂ŁA
		// c++11 �̊��̏ꍇ�A�K�Xstatic�ɓǂ݂����āA�N���X�O�ŏ��������Ă��������I

		class singleton_finalizer final {
			singleton_finalizer() = delete;
			static inline std::vector<std::function<void()>> finalizers;
		public:
			static void add_finalizer(std::function<void()>&& func) {
				finalizers.emplace_back(std::forward<decltype(func)>(func));
			}
			static void clear() noexcept {
				std::for_each(finalizers.crbegin(), finalizers.crend(), [](auto&& fn) {
					fn();
				}), finalizers.clear();
			}
		};

		template <typename T>
		class singleton_impl final {
			singleton_impl() = delete;
			static inline std::once_flag init_flag;
			static inline std::unique_ptr<T> instance;
			template <typename ... Args>
			static void get_singleton_impl(Args&& ... args) {
				instance = std::make_unique<T>(std::forward<Args>(args)...);
				singleton_finalizer::add_finalizer([] {instance.reset(nullptr); });
			}
		public:
			template <typename ... Args>
			static T& get_singleton(Args&& ... args) {
				std::call_once(init_flag, get_singleton_impl<Args...>, std::forward<Args>(args)...);
				return *instance.get();
			}
			static T* get() {
				return[ptr = instance.get()]
				{ return ptr ? ptr : throw std::logic_error("This type had not singleton object."); }();
			}
			static bool has_singleton() { return instance.get(); }
		};

	}  // namespace singleton_implNS

	class singleton final {
		singleton() = delete;
	public:
		template<typename T, typename ... Args>
		static T& get_singleton(Args&& ... args) {
			return singleton_implNS::singleton_impl<T>::get_singleton(std::forward<Args>(args)...);
		}
		template <typename T>
		static T& get() { return *singleton_implNS::singleton_impl<T>::get(); }
		template <typename T>
		static bool has_singleton() { return singleton_implNS::singleton_impl<T>::has_singleton(); }
		static void clear() noexcept { singleton_implNS::singleton_finalizer::clear(); }
	};

}  // namespace nyaruga_util

/*

class A {
public:
	A() { ++i; };
	explicit A(std::string j) : k(j) { ++i; }
	~A() { std::cout << "�R���X�g���N�^�̌Ă΂ꂽ��: " << i << "��" << "\n"; }
	std::string k;
	void a(int i) { std::cout << i << "   " << k << "\n"; }
	static inline int i;
};

int main() {
	// �R���X�g���N�^��"hello"��n���Ă��܂��BA::A(std::string)
	A& x = nyaruga_util::singleton::get_singleton<A>("hello");
	x.a(1);  // 1   hello

	// �V���O���g���Ȃ̂ŁAmake_singleton������Ă�ł��A�����I�u�W�F�N�g���A���Ă���悤�ɂ��Ă܂�
	// ���̏ꍇ�ɁA�����͂̒l�͖������Ă܂��B�ł����A�����̌^�ɍ����R���X�g���N�^���K�v�ł�
	A& z = nyaruga_util::singleton::get_singleton<A>("world");
	z.a(2);  // 2   hello
	A& w = nyaruga_util::singleton::get_singleton<A>();  // A::A() ���K�v�ł�
	w.a(3);  // 3   hello

	A& ww = nyaruga_util::singleton::get_singleton<A>(nyaruga_util::singleton::maked_tag{});  // A::A() ���K�v�ł�
	ww.a(3);  // 3   hello

	// �L���Ȓl��ێ����Ă��邩�𔻒肵�܂�
	if (nyaruga_util::singleton::has_singleton<A>()) {

		// ����Ȍ`�ŃI�u�W�F�N�g�����炦�܂�
		// �I�u�W�F�N�g������Ă��Ȃ��̂ɂɌĂԂƁAstd::logic_error ��O�𓊂��܂�
		A& y = nyaruga_util::singleton::get<A>();
		y.a(4);  // 4   hello
	}

	// ��������i�ꉞ�X�}�[�g�|�C���^�g���Ă���̂ŁA�Ȃ��Ă����������H�j
	// ������鏇�����d�v�Ȏ��͂�����ƌĂ�ł��������I
	nyaruga_util::singleton::clear();
}

*/