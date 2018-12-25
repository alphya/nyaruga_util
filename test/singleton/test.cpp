
#include <nyaruga_util/singleton.hpp>

// 量産シングルトンA
class MassSingletonA : public singleton<MassSingletonA>
{
	friend class singleton<MassSingletonA>;
	int     mData;
	MassSingletonA() : mData(12345) { }
public:
	void print(char const* iName)
	{
		std::cout << iName << ".print() mData = " << mData << " (" << &mData << ")\n";
	}
};

// 量産シングルトンB
class MassSingletonB : public singleton<MassSingletonB>
{
	friend class singleton<MassSingletonB>;
	std::string mData;
	MassSingletonB() : mData("default") { }
public:
	void print(char const* iName)
	{
		std::cout << iName << ".print() mData = " << mData << " (" << &mData << ")\n";
	}
};

int main()
{
	std::cout << "start of main()\n";

	auto& aMassSingletonA0 = MassSingletonA::get_instance();
	aMassSingletonA0.print("aMassSingletonA0");
	auto& aMassSingletonA1 = MassSingletonA::get_instance();
	aMassSingletonA1.print("aMassSingletonA1");

	auto& aMassSingletonB0 = MassSingletonB::get_instance();
	aMassSingletonB0.print("aMassSingletonB0");
	auto& aMassSingletonB1 = MassSingletonB::get_instance();
	aMassSingletonB1.print("aMassSingletonB1");

	std::cout << "end of main()\n";
}