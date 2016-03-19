#pragma once

#define QUOTE_SELECTANY __declspec(selectany)

#include <quote/pp/overload.hpp>

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
