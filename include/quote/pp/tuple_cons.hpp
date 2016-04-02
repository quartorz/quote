#pragma once

#include <quote/pp/tuple_unparenthesize.hpp>

#if !defined(QUOTE_PP_TUPLE_CONS)
#  define QUOTE_PP_TUPLE_CONS(elm, tuple) \
	QUOTE_PP_TUPLE_CONS_I(elm, QUOTE_PP_TUPLE_UNPARENTHESIZE(tuple))
#endif

#if !defined(QUOTE_PP_TUPLE_CONS_I)
#  define QUOTE_PP_TUPLE_CONS_I(elm, t) \
	QUOTE_PP_TUPLE_CONS_II(elm, t)
#endif

#if !defined(QUOTE_PP_TUPLE_CONS_II)
#  define QUOTE_PP_TUPLE_CONS_II(elm, ...) \
	(elm, __VA_ARGS__)
#endif
