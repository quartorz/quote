#pragma once

#include <quote/pp/size.hpp>
#include <quote/pp/cat.hpp>

#if !defined(QUOTE_PP_OVERLOAD)
#  define QUOTE_PP_OVERLOAD(name, ...) QUOTE_PP_CAT(QUOTE_PP_CAT(name, QUOTE_PP_SIZE(__VA_ARGS__)) (__VA_ARGS__),)
#endif
