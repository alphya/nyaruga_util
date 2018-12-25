

// テストしたもの　↓
#include <nyaruga_util/bind_other_than_select_arg.hpp>
// #include <nyaruga_util/bind_select_arg_ignore.hpp>
// #include <nyaruga_util/bind_select_arg_replace.hpp>


namespace nyaruga_util {};

using namespace nyaruga_util;

#include <iostream>

#define BOOST_TEST_MODULE Mytest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
	const int x = 1;
	BOOST_CHECK(x == 1);
}

//int main()
//{
	
//}