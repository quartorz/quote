#pragma once

#include "joystick.hpp"

namespace quote{

	template <class Traits>
	class scene_base: public Traits::template userdefined_object<scene_base<Traits>>
	{
	public:
		virtual void on_show() = 0;
		virtual void on_hide() = 0;
		virtual void on_key_down(unsigned keycode) = 0;
		virtual void on_key_up(unsigned keycode) = 0;
		virtual void on_timer(unsigned id) = 0;

#if defined(_MSC_VER)
		virtual void on_get_joystick_state(const joystick_id &id, const joystick_state &js)
		{
		}

		virtual void on_reload_joysticks(const std::vector<std::tuple<joystick_id, std::wstring>> &joysticks)
		{
		}
#endif
	};

}
