#pragma once

#include <quote/pp/cat.hpp>

#if !defined(QUOTE_PP_TUPLE_UNPARENTHESIZE)
#  define QUOTE_PP_TUPLE_UNPARENTHESIZE(tuple) \
	QUOTE_PP_CAT(QUOTE_PP_TUPLE_UNPARENTHESIZE_I tuple,)
#endif

#if !defined(QUOTE_PP_TUPLE_UNPARENTHESIZE_I)
#  define QUOTE_PP_TUPLE_UNPARENTHESIZE_I(...) __VA_ARGS__
#endif
