#pragma once

#if defined(_MSC_VER)

#include <dinput.h>

namespace quote{

	using joystick_id = GUID;
	using joystick_state = DIJOYSTATE2;

}

#endif
