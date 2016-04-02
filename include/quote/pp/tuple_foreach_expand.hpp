#pragma once

#include <quote/pp/cat.hpp>
#include <quote/pp/size.hpp>
#include <quote/pp/tuple_element.hpp>
#include <quote/pp/tuple_cons.hpp>
#include <quote/pp/tuple_car.hpp>
#include <quote/pp/tuple_cdr.hpp>

#if !defined(QUOTE_PP_TUPLE_FOREACH_EXPAND)
#  define QUOTE_PP_TUPLE_FOREACH_EXPAND(macro, data, tuple) \
	QUOTE_PP_CAT(QUOTE_PP_TUPLE_FOREACH_EXPAND_I_, QUOTE_PP_SIZE tuple)(macro, data, tuple)
#endif

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_0(macro, data, tuple) 

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_1(macro, data, tuple) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_II_1(macro, data, QUOTE_PP_TUPLE_ELEMENT(0, tuple))
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_II_1(macro, data, elm) \
	 QUOTE_PP_TUPLE_FOREACH_EXPAND_III_1(macro, data, elm)
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_III_1(macro, data, elm) \
	 macro(data, elm)

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_2(macro, data, tuple) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_II_2(macro, data, QUOTE_PP_TUPLE_CAR(tuple), QUOTE_PP_TUPLE_CDR(tuple))
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_II_2(macro, data, elm, rest) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_III_2(macro, data, elm, rest)
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_III_2(macro, data, elm, rest) \
	macro(data, elm) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_I_1(macro, data, rest)

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_3(macro, data, tuple) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_II_3(macro, data, QUOTE_PP_TUPLE_CAR(tuple), QUOTE_PP_TUPLE_CDR(tuple))
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_II_3(macro, data, elm, rest) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_III_3(macro, data, elm, rest)
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_III_3(macro, data, elm, rest) \
	macro(data, elm) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_I_2(macro, data, rest)

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_4(macro, data, tuple) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_II_4(macro, data, QUOTE_PP_TUPLE_CAR(tuple), QUOTE_PP_TUPLE_CDR(tuple))
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_II_4(macro, data, elm, rest) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_III_4(macro, data, elm, rest)
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_III_4(macro, data, elm, rest) \
	macro(data, elm) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_I_3(macro, data, rest)

#define QUOTE_PP_TUPLE_FOREACH_EXPAND_I_5(macro, data, tuple) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_II_5(macro, data, QUOTE_PP_TUPLE_CAR(tuple), QUOTE_PP_TUPLE_CDR(tuple))
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_II_5(macro, data, elm, rest) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_III_5(macro, data, elm, rest)
#define QUOTE_PP_TUPLE_FOREACH_EXPAND_III_5(macro, data, elm, rest) \
	macro(data, elm) \
	QUOTE_PP_TUPLE_FOREACH_EXPAND_I_4(macro, data, rest)
