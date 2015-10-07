#pragma once

namespace quote{
	bool initialize();
	void uninitialize();
}

#include "impl/quote.hpp"

#if defined _MSC_VER

#include <objbase.h>

#define QUOTE_DEFINE_MAIN \
	int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) \
	{ \
		if(FAILED(::CoInitialize(NULL)) || !::quote::initialize()) \
			return 0; \
		int r = ::run(); \
		::quote::uninitialize(); \
		::CoUninitialize(); \
		return r; \
	}

#endif
