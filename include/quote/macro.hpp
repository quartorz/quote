#pragma once

#define QUOTE_SELECTANY __declspec(selectany)

#include <quote/pp/overload.hpp>

#define QUOTE_DECLARE_BINDER(...) QUOTE_PP_OVERLOAD(QUOTE_DECLARE_BINDER_, __VA_ARGS__)

#define QUOTE_DECLARE_BINDER_2(parent, funcname) QUOTE_DECLARE_BINDER_3(parent, funcname, funcname ## _binder)
#define QUOTE_DECLARE_BINDER_3(parent, funcname, bindername) \
	struct bindername { \
		using type = parent; \
		template <class U, class... Args> \
		auto operator()(U *obj, parent *this_, Args... args) -> decltype(obj->funcname(*this_, args...)) \
		{ \
			return obj->funcname(*this_, args...); \
		} \
		template <class U> \
		void operator()(U *, ...) \
		{ \
		} \
	};
