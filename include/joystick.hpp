#pragma once

#if defined(_MSC_VER)

#if !defined(DIRECTINPUT_VERSION)
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

namespace quote{

	using joystick_id = GUID;
	using joystick_state = DIJOYSTATE2;

}

#endif
