#pragma once

#include <quote/pp/overload.hpp>

#if !defined(QUOTE_PP_IF)
#  define QUOTE_PP_IF(cond, if_, else_) \
	QUOTE_PP_CAT(QUOTE_PP_CAT(QUOTE_PP_IF_I_, cond)(if_, else_),)
#endif

#if !defined(QUOTE_PP_IF_I_0)
#  define QUOTE_PP_IF_I_0(if_, else_) else_
#endif

#if !defined(QUOTE_PP_IF_I_1)
#  define QUOTE_PP_IF_I_1(if_, else_) if_
#endif
