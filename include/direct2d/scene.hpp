#pragma once

#include "../scene.hpp"
#include "traits.hpp"

#if !defined DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

namespace quote{ namespace direct2d{

	class scene: public ::quote::scene<traits>{
	protected:
		using joystick_id = GUID;
		using joystick_state = DIJOYSTATE2;

	public:
		virtual void on_get_joystick_state(const joystick_id &id, const joystick_state &js)
		{
		}
		virtual void on_reload_joysticks(const std::vector<std::tuple<joystick_id, std::wstring>> &joysticks)
		{
		}
	};

} }
