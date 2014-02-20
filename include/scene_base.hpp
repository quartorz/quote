#pragma once

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
	};

}
