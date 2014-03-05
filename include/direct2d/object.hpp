#pragma once

#include "base_types.hpp"
#include "resource.hpp"

namespace quote{ namespace direct2d{

	class object: public resource{
	protected:
		point pos;
		size size_;

	public:
		struct hittest{
			HWND hwnd;

			enum class cursor: int{
				arrow,
				hand,
				ibeam,
				leftright,
			}cursor_;

			void set_cursor(cursor c)
			{
				cursor_ = c;
			}
		};
		template <class Window>
		static hittest create_hittest(Window *w)
		{
			return {w->get_hwnd(), hittest::cursor::arrow};
		}

		virtual bool is_colliding(const point &ap)
		{
			return ap.is_inside(get_drawing_rect());
		}
		virtual void on_left_press(const point &, hittest &)
		{
		}
		virtual void on_left_release(const point &, hittest &)
		{
		}
		virtual void on_mouse_move(const point &, hittest &)
		{
		}
		virtual void on_mouse_leave(const hittest &)
		{
		}
		virtual void on_lose_focus(const hittest &)
		{
		}

		virtual void draw(const paint_params &) = 0;

		virtual rect get_drawing_rect()
		{
			return get_rect();
		}
		virtual void set_position(const point &p)
		{
			pos = p;
		}
		virtual void set_size(const size &s)
		{
			size_ = s;
		}
		virtual void set_rect(const rect &r)
		{
			set_position(point(r.left, r.top));
			set_size(size(r.width(), r.height()));
		}
		virtual point get_position()
		{
			return pos;
		}
		virtual size get_size()
		{
			return size_;
		}
		rect get_rect()
		{
			return rect(get_position(), get_size());
		}
	};

} }
