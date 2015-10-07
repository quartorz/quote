#pragma once

#include "../win32/utils.hpp"

#include <dwrite.h>

namespace quote{ namespace utils{

	DWRITE_TEXT_RANGE CreateRange(int start, int end)
	{
		return {start, end - start};
	}

} }
