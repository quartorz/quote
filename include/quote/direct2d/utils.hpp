#pragma once

#include <quote/win32/utils.hpp>

#include <dwrite.h>

namespace quote{ namespace utils{

	DWRITE_TEXT_RANGE CreateRange(int start, int end)
	{
		return {static_cast<UINT32>(start), static_cast<UINT32>(end - start)};
	}

} }
