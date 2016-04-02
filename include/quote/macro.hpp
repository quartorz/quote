#pragma once

#define QUOTE_SELECTANY __declspec(selectany)

#include <quote/pp/overload.hpp>
#include <quote/pp/if.hpp>
#include <quote/pp/is_known_token.hpp>
#include <quote/pp/tuple_expand.hpp>
#include <quote/pp/tuple_foreach.hpp>

#define QUOTE_DEFINE_BINDER(...) QUOTE_PP_OVERLOAD(QUOTE_DEFINE_BINDER_, __VA_ARGS__)
#define QUOTE_DEFINE_BINDER_2(parent, funcname) QUOTE_DEFINE_BINDER_3(parent, funcname, funcname ## _binder)

#include <quote/tmp/print.hpp>

#define QUOTE_DEFINE_BINDER_3(parent, funcname, bindername) \
	struct bindername { \
		using type = parent; \
		template <bool Debug = false, typename T, typename ... Args> \
		auto operator()(T *obj, parent *this_, Args... args) -> decltype(obj->funcname(*this_, args...)) \
		{ \
			return obj->funcname(*this_, args...); \
		} \
		template <bool Debug = false, typename T, typename ... Us> \
		auto operator()(T *, Us...) \
			-> ::std::enable_if_t<Debug> \
		{ \
			using type = ::quote::tmp::print_t<::std::tuple<T, Us...>>; \
		} \
		template <bool Debug = false, typename T, typename ... Us> \
		auto operator()(T *, Us...) \
			-> ::std::enable_if_t<!Debug> \
		{ \
		} \
	};

#if !defined(QUOTE_PP_TOKEN_const_getter)
#  define QUOTE_PP_TOKEN_getter ,
#endif

#if !defined(QUOTE_PP_TOKEN_getter)
#  define QUOTE_PP_TOKEN_getter ,
#endif

#if !defined(QUOTE_PP_TOKEN_setter)
#  define QUOTE_PP_TOKEN_setter ,
#endif

#define QUOTE_DEFINE_SIMPLE_PROPERTY(...) \
	QUOTE_PP_OVERLOAD(QUOTE_DEFINE_SIMPLE_PROPERTY_I_, __VA_ARGS__)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_I_2(type, name) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_I_3(type, name, (getter, const_getter, setter))

#define QUOTE_DEFINE_SIMPLE_PROPERTY_I_3(type, name, tuple) \
	private: \
		type name ## _; \
	public: \
		QUOTE_PP_TUPLE_EXPAND( \
			QUOTE_PP_TUPLE_FOREACH(QUOTE_DEFINE_SIMPLE_PROPERTY_DISPATCHER, (type, name), tuple))

#define QUOTE_DEFINE_SIMPLE_PROPERTY_DISPATCHER(data, elm) \
	QUOTE_PP_IF(QUOTE_PP_IS_KNOWN_TOKEN(elm), \
		QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, elm) data,)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_getter(type, name) \
	type &name() \
	{ \
		return name ## _; \
	}

#define QUOTE_DEFINE_SIMPLE_PROPERTY_const_getter(type, name) \
	const type &name() const \
	{ \
		return name ## _; \
	}

#define QUOTE_DEFINE_SIMPLE_PROPERTY_setter(type, name) \
	void name(const type &value) \
	{ \
		name ## _ = value; \
	}
