#pragma once

#include "../scene.hpp"
#include "traits.hpp"

#include "scene_base.hpp"

namespace quote{ namespace direct2d{

	template <class... Options>
	class scene: public ::quote::scene<traits, Options...>{
	protected:
		using joystick_id = GUID;
		using joystick_state = DIJOYSTATE2;

	public:
		virtual void on_get_joystick_state(const joystick_id &id, const joystick_state &js) override
		{
		}
		virtual void on_reload_joysticks(const std::vector<std::tuple<joystick_id, std::wstring>> &joysticks) override
		{
		}
	};

} }
