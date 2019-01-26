
#define BOOST_TEST_MODULE singleton_test
#include <boost/test/unit_test.hpp>

#include <nyaruga_util/singleton.hpp>
#include <string>

using namespace nyaruga_util;

// 量産シングルトンA
struct mass_singleton_a : public singleton<mass_singleton_a> {
   friend class singleton<mass_singleton_a>;
   int m_data;
   mass_singleton_a() : m_data(12345) {}
};

// 量産シングルトンB
struct mass_singleton_b : public singleton<mass_singleton_b> {
   friend class singleton<mass_singleton_b>;
   std::string m_data;
   mass_singleton_b() : m_data("default") {}
};

BOOST_AUTO_TEST_CASE(singleton_test_1)
{
   auto & a_mass_singleton_a0 = mass_singleton_a::get_instance();
   BOOST_CHECK(a_mass_singleton_a0.m_data == 12345);
   a_mass_singleton_a0.m_data = 100;

   auto & a_mass_singleton_a1 = mass_singleton_a::get_instance();
   BOOST_CHECK(a_mass_singleton_a1.m_data == 100);

   auto & a_mass_singleton_b0 = mass_singleton_b::get_instance();
   BOOST_CHECK(a_mass_singleton_b0.m_data == "default");
   a_mass_singleton_b0.m_data = "abc";

   auto & a_mass_singleton_b1 = mass_singleton_b::get_instance();
   BOOST_CHECK(a_mass_singleton_b1.m_data == "abc");
}