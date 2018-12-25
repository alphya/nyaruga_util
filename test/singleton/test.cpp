
#define BOOST_TEST_MODULE singleton_test
#include <boost/test/unit_test.hpp>

#include <nyaruga_util/singleton.hpp>
#include <string>

using namespace nyaruga_util;

// 量産シングルトンA
struct MassSingletonA : public singleton<MassSingletonA>
{
	friend class singleton<MassSingletonA>;
	int     mData;
	MassSingletonA() : mData(12345) { }
};

// 量産シングルトンB
struct MassSingletonB : public singleton<MassSingletonB>
{
	friend class singleton<MassSingletonB>;
	std::string mData;
	MassSingletonB() : mData("default") { }
};


BOOST_AUTO_TEST_CASE(singleton_test_1)
{
	auto& aMassSingletonA0 = MassSingletonA::get_instance();
	BOOST_CHECK(aMassSingletonA0.mData == 12345);
	aMassSingletonA0.mData = 100;

	auto& aMassSingletonA1 = MassSingletonA::get_instance();
	BOOST_CHECK(aMassSingletonA1.mData == 100);

	auto& aMassSingletonB0 = MassSingletonB::get_instance();
	BOOST_CHECK(aMassSingletonB0.mData == "default");
	aMassSingletonB0.mData = "abc";

	auto& aMassSingletonB1 = MassSingletonB::get_instance();
	BOOST_CHECK(aMassSingletonB1.mData == "abc");
}