#pragma once

#if !defined(QUOTE_PP_STRINGIZE)
#  define QUOTE_PP_STRINGIZE(a) \
	QUOTE_PP_STRINGIZE_I(a)
#endif

#if !defined(QUOTE_PP_STRINGIZE_I)
#  define QUOTE_PP_STRINGIZE_I(a) \
	QUOTE_PP_STRINGIZE_II(a)
#endif

#if !defined(QUOTE_PP_STRINGIZE_II)
#  define QUOTE_PP_STRINGIZE_II(a) \
	# a
#endif
