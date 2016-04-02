#pragma once

#define QUOTE_SELECTANY __declspec(selectany)

#include <quote/pp/overload.hpp>
#include <quote/pp/if.hpp>
#include <quote/pp/is_known_token.hpp>
#include <quote/pp/tuple_foreach_expand.hpp>
#include <quote/pp/size.hpp>

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

#if !defined(QUOTE_PP_TOKEN_empty)
#  define QUOTE_PP_TOKEN_getter ,
#endif

#if !defined(QUOTE_PP_TOKEN_default)
#  define QUOTE_PP_TOKEN_default ,
#endif

#if !defined(QUOTE_PP_TOKEN_accessor)
#  define QUOTE_PP_TOKEN_accessor ,
#endif

#if !defined(QUOTE_PP_TOKEN_accessors)
#  define QUOTE_PP_TOKEN_accessors ,
#endif

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
	QUOTE_DEFINE_SIMPLE_PROPERTY_I_4(type, name, accessors (getter, const_getter, setter), empty ())

#define QUOTE_DEFINE_SIMPLE_PROPERTY_I_3(type, name, v) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_I_4(type, name, v, empty ())

#define QUOTE_DEFINE_SIMPLE_PROPERTY_I_4(type, name, v0, v1) \
	private: \
		type name ## _ \
		QUOTE_PP_TUPLE_FOREACH_EXPAND(QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_DEFAULT, , (v0, v1)) \
		; \
	public: \
		QUOTE_PP_TUPLE_FOREACH_EXPAND(QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS, (type, name), (v0, v1))

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_DEFAULT(data, elm) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_DEFAULT_DISPATCHER_, elm),)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_DEFAULT_DISPATCHER_empty(...)
#define QUOTE_DEFINE_SIMPLE_PROPERTY_DEFAULT_DISPATCHER_accessor(...)
#define QUOTE_DEFINE_SIMPLE_PROPERTY_DEFAULT_DISPATCHER_accessors(...)
#define QUOTE_DEFINE_SIMPLE_PROPERTY_DEFAULT_DISPATCHER_default(value) = value

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS(data, elm) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_I( \
		QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_ACCESSORS_DISPATCHER_, elm),), data)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_ACCESSORS_DISPATCHER_empty(...) ()
#define QUOTE_DEFINE_SIMPLE_PROPERTY_ACCESSORS_DISPATCHER_default(...) ()
#define QUOTE_DEFINE_SIMPLE_PROPERTY_ACCESSORS_DISPATCHER_accessor(...) (__VA_ARGS__)
#define QUOTE_DEFINE_SIMPLE_PROPERTY_ACCESSORS_DISPATCHER_accessors(...) (__VA_ARGS__)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_I(tuple, data) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_II(QUOTE_PP_CAT(QUOTE_PP_SIZE tuple,), tuple, data)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_II(size, tuple, data) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_III_, size)(data, tuple),)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_III_0(data, tuple) \

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_III_1(data, tuple) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_1(QUOTE_PP_TUPLE_ELEMENT(0, tuple), data)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_1(v0, data) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v0) data,)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_III_2(data, tuple) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_1( \
		QUOTE_PP_TUPLE_ELEMENT(0, tuple), QUOTE_PP_TUPLE_ELEMENT(1, tuple), data)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_2(v0, v1, data) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v0) data,) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v1) data,)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_III_3(data, tuple) \
	QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_1( \
		QUOTE_PP_TUPLE_ELEMENT(0, tuple), QUOTE_PP_TUPLE_ELEMENT(1, tuple), QUOTE_PP_TUPLE_ELEMENT(2, tuple), data)

#define QUOTE_DEFINE_SIMPLE_PROPERTY_MAKE_ACCESSORS_IIII_3(v0, v1, v2, data) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v0) data,) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v1) data,) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_DEFINE_SIMPLE_PROPERTY_, v2) data,)

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
