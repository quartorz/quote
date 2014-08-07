#pragma once

#include <type_traits>

#define QUOTE_DECL_HAS_TYPE(t) \
	template <class Type>\
	class has_ ## t{\
		template <class T, class U = typename T::t>\
		static std::true_type check(int);\
		template <class T>\
		static std::false_type check(long);\
	public:\
		using type = decltype(check<Type>(0));\
		static const bool value = std::common_type<decltype(check<Type>(0))>::type::value;\
	};

#define QUOTE_DECL_HAS_NON_TYPE(f)\
	template <class Type>\
	class has_ ## f{\
		template <class T, decltype(&T::f) = &T::f>\
		static std::true_type check(int);\
		template <class T>\
		static std::false_type check(long);\
	public:\
		using type = decltype(check<Type>(0));\
		static const bool value = std::common_type<decltype(check<Type>(0))>::type::value;\
	};
