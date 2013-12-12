#pragma once

namespace quote{

	template <class Traits>
	class button: public Traits::object{
	public:
		enum class State: int{
			None,
			Hover,
			Push,
		};

	protected:
		State state = State::None;
		bool pushing = false;
		virtual void set_state(State );

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

#include "details\button.hpp"
