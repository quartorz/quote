#pragma once

#include <quote/pp/cat.hpp>
#include <quote/pp/size.hpp>

#if !defined(QUOTE_PP_TUPLE_EXPAND)
#  define QUOTE_PP_TUPLE_EXPAND(tuple) \
	QUOTE_PP_TUPLE_EXPAND_I(QUOTE_PP_SIZE tuple, tuple)
#endif

#define QUOTE_PP_TUPLE_EXPAND_I(size, tuple) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_PP_TUPLE_EXPAND_II_, size) tuple,)

#define QUOTE_PP_TUPLE_EXPAND_II_0()
#define QUOTE_PP_TUPLE_EXPAND_II_1(e0) \
	e0
#define QUOTE_PP_TUPLE_EXPAND_II_2(e0, e1) \
	e0 e1
#define QUOTE_PP_TUPLE_EXPAND_II_3(e0, e1, e2) \
	e0 e1 e2
#define QUOTE_PP_TUPLE_EXPAND_II_4(e0, e1, e2, e3) \
	e0 e1 e2 e3
#define QUOTE_PP_TUPLE_EXPAND_II_5(e0, e1, e2, e3, e4) \
	e0 e1 e2 e3 e4
