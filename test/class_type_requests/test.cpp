#define BOOST_TEST_MODULE class_type_requests_test
#include <boost/test/unit_test.hpp>

#include "../../nyaruga_util/class_type_requests.hpp"
#include <type_traits>

using namespace nyaruga_util;
using namespace std;

class my_class
   : this_class<is_class<my_class>> {
};

BOOST_AUTO_TEST_CASE(class_type_requests_test_1)
{
   BOOST_CHECK(true);
}
