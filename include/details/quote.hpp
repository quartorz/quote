#pragma once

#include "../quote.hpp"

#include "../win32/init.hpp"
#include "../direct2d/init.hpp"

namespace quote{

	bool initialize()
	{
		return win32::initialize() && direct2d::initialize();
	}

	void uninitialize()
	{
		win32::uninitialize();
		direct2d::uninitialize();
	}

}
