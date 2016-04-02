#pragma once

#if !defined(QUOTE_PP_TUPLE_CDR)
#  define QUOTE_PP_TUPLE_CDR(tuple) \
	QUOTE_PP_TUPLE_CDR_I tuple
#endif

#if !defined(QUOTE_PP_TUPLE_CDR_I)
#  define QUOTE_PP_TUPLE_CDR_I(a, ...) (__VA_ARGS__)
#endif
