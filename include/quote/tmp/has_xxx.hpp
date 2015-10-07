#pragma once

#include <type_traits>

#define QUOTE_DECL_HAS_TYPE(t) \
	template <class Type>\
	class has_ ## t{\
		template <class T, class U = typename T::t>\
		static ::std::true_type check(int);\
		template <class T>\
		static ::std::false_type check(long);\
	public:\
		using type = decltype(check<Type>(0));\
		static const bool value = type::value;\
	};

#define QUOTE_DECL_HAS_NON_TYPE(f)\
	template <class Type>\
	class has_ ## f{\
		template <class T>\
		static decltype(&::std::common_type<decltype(::std::declval<Type>())>::type::f, void(), ::std::true_type()) check(T);\
		static ::std::false_type check(...);\
	public:\
		using type = decltype(check(::std::declval<Type>()));\
		static const bool value = type::value;\
	};
