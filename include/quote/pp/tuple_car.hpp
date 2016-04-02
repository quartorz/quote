#pragma once

#if !defined(QUOTE_PP_TUPLE_CAR)
#  define QUOTE_PP_TUPLE_CAR(tuple) \
	QUOTE_PP_TUPLE_CAR_I tuple
#endif

#if !defined(QUOTE_PP_TUPLE_CAR_I)
#  define QUOTE_PP_TUPLE_CAR_I(a, ...) a
#endif
