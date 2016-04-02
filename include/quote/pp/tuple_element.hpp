#pragma once

#include <quote/pp/overload.hpp>

#if !defined(QUOTE_PP_TUPLE_ELEMENT)
//#  define QUOTE_PP_TUPLE_ELEMENT(index, tuple) \
//	QUOTE_PP_TUPLE_ELEMENT_I(index, tuple)
#endif

#if !defined(QUOTE_PP_TUPLE_ELEMENT_I)
#  define QUOTE_PP_TUPLE_ELEMENT(index, tuple) \
	QUOTE_PP_CAT(QUOTE_PP_TUPLE_ELEMENT_II_, index) tuple
#endif

#define QUOTE_PP_TUPLE_ELEMENT_II_0(e0, ...) e0
#define QUOTE_PP_TUPLE_ELEMENT_II_1(e0, e1, ...) e1
#define QUOTE_PP_TUPLE_ELEMENT_II_2(e0, e1, e2, ...) e2
#define QUOTE_PP_TUPLE_ELEMENT_II_3(e0, e1, e2, e3, ...) e3
#define QUOTE_PP_TUPLE_ELEMENT_II_4(e0, e1, e2, e3, e4, ...) e4
#define QUOTE_PP_TUPLE_ELEMENT_II_5(e0, e1, e2, e3, e4, e5, ...) e5
#define QUOTE_PP_TUPLE_ELEMENT_II_6(e0, e1, e2, e3, e4, e5, e6, ...) e6
#define QUOTE_PP_TUPLE_ELEMENT_II_7(e0, e1, e2, e3, e4, e5, e6, e7, ...) e7
#define QUOTE_PP_TUPLE_ELEMENT_II_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, ...) e8
#define QUOTE_PP_TUPLE_ELEMENT_II_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9
