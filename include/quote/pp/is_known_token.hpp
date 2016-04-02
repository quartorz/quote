#pragma once

#include <quote/pp/tuple_element.hpp>
#include <quote/pp/is_empty.hpp>
#include <quote/pp/cat.hpp>

/*

"token"という名前のトークンを登録するには
#define QUOTE_PP_TOKEN_token ,
とする

*/

#if !defined(QUOTE_PP_IS_KNOWN_TOKEN)
#  define QUOTE_PP_IS_KNOWN_TOKEN(token) \
	QUOTE_PP_IS_KNOWN_TOKEN_I(QUOTE_PP_TUPLE_ELEMENT(0, (QUOTE_PP_CAT(QUOTE_PP_TOKEN_, token))))
#endif

#if !defined(QUOTE_PP_IS_KNOWN_TOKEN_I)
#  define QUOTE_PP_IS_KNOWN_TOKEN_I(a) \
	QUOTE_PP_IS_EMPTY(a)
#endif
