#pragma once

namespace quote{

	template <class Traits>
	class button: public Traits::object{
	public:
		enum class state: int{
			none,
			hover,
			push,
		};

	protected:
		state state_ = state::none;
		bool pushing = false;
		virtual void set_state(state);

	public:
		~button();

		virtual void on_push() = 0;

		virtual void on_left_press(const typename Traits::point &, typename Traits::object::hittest &) override;
		virtual void on_left_release(const typename Traits::point &, typename Traits::object::hittest &) override;
		virtual void on_mouse_move(const typename Traits::point &, typename Traits::object::hittest &) override;
		virtual void on_mouse_leave(const typename Traits::object::hittest &) override;
		virtual void on_lose_focus(const typename Traits::object::hittest &) override;
	};

}

#include "impl/button.hpp"
