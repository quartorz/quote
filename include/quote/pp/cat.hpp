#pragma once

#if !defined(QUOTE_PP_CAT)
# define QUOTE_PP_CAT(a, b) QUOTE_PP_CAT_I(a, b)
#endif

#if !defined(QUOTE_PP_CAT_I)
# define QUOTE_PP_CAT_I(a, b) a ## b
#endif