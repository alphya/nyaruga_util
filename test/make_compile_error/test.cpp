
#define BOOST_TEST_MODULE make_compile_error_test
#include <boost/test/included/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_CASE(make_compile_error_test_1)
{
   [[maybe_unused]] auto ll = [](auto &&... hh) {
      return 0;
   };

   // BOOST_CHECK(ll(9, 10, 11) == 10);
   BOOST_CHECK(true);
}

/* コメントアウトを外した時のコンパイル時出力

  NYARUGA_UTIL MAKED COMPILE ERROR!
  file: "D:\\Desktop\\nyaruga_util\\nyaruga_util/pac_select_pos_obj.hpp"
  line: 26
  message: "pos is out of pac"

*/