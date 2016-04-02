#pragma once

#include <quote/pp/tuple_unparenthesize.hpp>

#if !defined(QUOTE_PP_TUPLE_CAT)
#  define QUOTE_PP_TUPLE_CAT(t1, t2) \
	QUOTE_PP_TUPLE_CAT_I( \
		QUOTE_PP_TUPLE_UNPARENTHESIZE(t1), QUOTE_PP_TUPLE_UNPARENTHESIZE(t2))
#endif

#if !defined(QUOTE_PP_TUPLE_CAT_I)
#  define QUOTE_PP_TUPLE_CAT_I(...) (__VA_ARGS__)
#endif
