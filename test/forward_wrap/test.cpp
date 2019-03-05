#define BOOST_TEST_MODULE forward_wrap_test
#include <boost/test/unit_test.hpp>

#include "../../nyaruga_util/forward_wrap.hpp"
#include <type_traits>

static int constructed = 0;
static int destructed = 0;

struct my_struct {
   my_struct() noexcept { ++constructed; };
   ~my_struct() { ++destructed; };
   my_struct(my_struct const &) noexcept { ++constructed; };
   my_struct(my_struct &&) noexcept { ++constructed; };
};

template <typename T>
T && func(T && arg) noexcept
{
   return nyaruga::util::forward(arg);
}

BOOST_AUTO_TEST_CASE(forward_wrap_test_1)
{
   {
      my_struct m;
      func(func(func(func(func(func(m))))));
   }
   BOOST_CHECK(constructed == 1);
   BOOST_CHECK(destructed == 1);
}
