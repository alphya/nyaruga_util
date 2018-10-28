#pragma once

// ���̃R�[�h�́Ahttps://cdecrement.blog.fc2.com/blog-entry-234.html
// ���炢�������Ă��������̂ł��B
// ���肪�����g�킹�Ă�����Ă��܂��B���肪�Ƃ��������܂��B

namespace nyaruga_util {

namespace nyaruga_util_impl
{

//�e���v���[�g�p�����[�^�̎w�肵���ʒu�̌^���擾
template<int N, typename... T>
struct get_template_param {
	//�����̐�������Ȃ�
	using type = void;
};
//�e���v���[�g�p�����[�^����ȏ�̎��̕������ꉻ
template<int N, typename First, typename... Rest>
struct get_template_param<N, First, Rest...> {
	using type = std::conditional_t<N == 0, First, 
		typename get_template_param<N - 1, Rest...>::type>;
};

}  // nyaruga_util_impl

//�֐��̈����̌^�𓾂�
template<typename Func, int N>
struct get_function_argument_type {
};
//�����o�֐��p���ꉻ
template<class C, typename Ret, typename... Args, int N>
struct get_function_argument_type<Ret(C::*)(Args...), N> {
	using type = typename nyaruga_util_impl::get_template_param<N-1, Args...>::type;
};
//�ʏ�֐��p���ꉻ
template<typename Ret, typename... Args, int N>
struct get_function_argument_type<Ret(*)(Args...), N> {
	using type = typename nyaruga_util_impl::get_template_param<N-1, Args...>::type;
};

//get_function_argument_type�̃G�C���A�X�e���v���[�g��
template<typename Func, int N>
using get_function_argument_type_t = typename get_function_argument_type<Func, N>::type;

}  // nyaruga_util

/*

class Test{
public:
	void memfunc(bool,int,double,std::string);
};

void func(bool,int,double,std::string);

int main() {
	std::cout << typeid(get_function_argument_type_t<decltype(&Test::memfunc),1>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&func),2>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&Test::memfunc),3>).name() << std::endl;
	std::cout << typeid(get_function_argument_type_t<decltype(&func),5>).name() << std::endl;
	return 0;
}

*/