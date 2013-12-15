#pragma once

#include <memory>

#if !defined SELECTANY
#if defined _MSC_VER
#define SELECTANY __declspec(selectany)
#elif defined __GNUC__
#define SELECTANY __attribute__((selectany))
#endif
#endif

template <class Derived>
class singleton{
protected:
	static std::unique_ptr<Derived> instance;

public:
	static Derived *get_instance()
	{
		if(!instance){
			instance = std::unique_ptr<Derived>(new Derived);
		}
		return instance.get();
	}
};

template <class Derived>
SELECTANY std::unique_ptr<Derived> singleton<Derived>::instance;
