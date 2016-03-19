#pragma once

#include <quote/pp/cat.hpp>
#include <quote/pp/is_empty.hpp>

#if !defined(QUOTE_PP_SIZE)
#  define QUOTE_PP_SIZE(...) QUOTE_PP_CAT(QUOTE_PP_SIZE_I(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1),)
#endif

#if !defined(QUOTE_PP_SIZE_I)
#  define QUOTE_PP_SIZE_I(e1, e2, e3, e4, e5, e6, e7, e8, e9, size, ...) QUOTE_PP_CAT(QUOTE_PP_SIZE_II, QUOTE_PP_IS_EMPTY(e1)) (0, size)
#endif

#if !defined(QUOTE_PP_SIZE_II1)
#  define QUOTE_PP_SIZE_II1(a, b) a
#endif

#if !defined(QUOTE_PP_SIZE_II0)
#  define QUOTE_PP_SIZE_II0(a, b) b
#endif
