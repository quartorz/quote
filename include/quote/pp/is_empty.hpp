#pragma once

#include <quote/pp/cat.hpp>

#if !defined(QUOTE_PP_EMPTY)
#  define QUOTE_PP_IS_EMPTY(...) QUOTE_PP_IS_EMPTY_II(QUOTE_PP_CAT(QUOTE_PP_IS_EMPTY_I ## __VA_ARGS__,), 0)
#endif

#if !defined(QUOTE_PP_IS_EMPTY_I)
#  define QUOTE_PP_IS_EMPTY_I _, 1
#endif

#if !defined(QUOTE_PP_IS_EMPTY_II)
#  define QUOTE_PP_IS_EMPTY_II(a, b, ...) QUOTE_PP_CAT(QUOTE_PP_IS_EMPTY_III(a, b, __VA_ARGS__),)
#endif

#if !defined(QUOTE_PP_IS_EMPTY_III)
#  define QUOTE_PP_IS_EMPTY_III(a, b, ...) b
#endif
