#pragma once

#include "../button.hpp"

namespace quote{

	template <class Traits>
	void button<Traits>::set_state(state s)
	{
		state_ = s;
	}

	template <class Traits>
	button<Traits>::~button()
	{
	}

	template <class Traits>
	void button<Traits>::on_left_press(const typename Traits::point &ap, typename Traits::object::hittest &ht)
	{
		ht.set_cursor(Traits::object::hittest::cursor::hand);
		pushing = true;
		set_state(state::push);
	}

	template <class Traits>
	void button<Traits>::on_left_release(const typename Traits::point &ap, typename Traits::object::hittest &ht)
	{
		pushing = false;
		if(this->is_colliding(ap)){
			on_push();
			set_state(state::hover);
		}else
			set_state(state::none);
	}

	template <class Traits>
	void button<Traits>::on_mouse_move(const typename Traits::point &ap, typename Traits::object::hittest &ht)
	{
		if(pushing){
			ht.set_cursor(Traits::object::hittest::cursor::hand);
			if(this->is_colliding(ap)){
				// 押されててカーソルが中にある
				set_state(state::push);
			}else{
				// 押されてるけど外にある
				set_state(state::hover);
			}
		}else{
			if(is_colliding(ap)){
				// 押してないけどカーソルが乗ってる=ホバーしてる
				set_state(state::hover);
				ht.set_cursor(Traits::object::hittest::cursor::hand);
			}else
				set_state(state::none);
		}
	}

	template <class Traits>
	void button<Traits>::on_mouse_leave(const typename Traits::object::hittest &)
	{
		set_state(state::none);
	}

	template <class Traits>
	void button<Traits>::on_lose_focus(const typename Traits::object::hittest &)
	{
		set_state(state::none);
	}

}
