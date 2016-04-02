#pragma once

#include <quote/pp/cat.hpp>
#include <quote/pp/size.hpp>

#if !defined(QUOTE_PP_TUPLE_EXPAND)
#  define QUOTE_PP_TUPLE_EXPAND(tuple) \
	QUOTE_PP_TUPLE_EXPAND_I(QUOTE_PP_SIZE tuple, tuple)
#endif

#define QUOTE_PP_TUPLE_EXPAND_I(size, tuple) \
	QUOTE_PP_TUPLE_EXPAND_II(QUOTE_PP_CAT(QUOTE_PP_TUPLE_EXPAND_III_, size), tuple)

#define QUOTE_PP_TUPLE_EXPAND_II(name, tuple) \
	QUOTE_PP_CAT(name tuple,)

#define QUOTE_PP_TUPLE_EXPAND_III_0()
#define QUOTE_PP_TUPLE_EXPAND_III_1(e0) \
	e0
#define QUOTE_PP_TUPLE_EXPAND_III_2(e0, e1) \
	e0 e1
#define QUOTE_PP_TUPLE_EXPAND_III_3(e0, e1, e2) \
	e0 e1 e2
#define QUOTE_PP_TUPLE_EXPAND_III_4(e0, e1, e2, e3) \
	e0 e1 e2 e3
#define QUOTE_PP_TUPLE_EXPAND_III_5(e0, e1, e2, e3, e4) \
	e0 e1 e2 e3 e4
