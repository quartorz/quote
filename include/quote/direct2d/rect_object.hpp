#pragma once

#include "base_types.hpp"
#include "userdefined_object.hpp"

#include "solid_brush.hpp"

namespace quote{ namespace direct2d{

	class rect_object: public userdefined_object<rect_object>, public rect{
		solid_brush border, fill;
		float border_width;

	public:
		rect_object(float left=0.f, float top=0.f, float right=0.f, float bottom=0.f)
			: rect(left, top, right, bottom)
			, border(color(255, 255, 255))
			, fill(color(0, 0, 0, 0))
			, border_width(1.f)
		{
			register_resource(&border);
			register_resource(&fill);
		}
		rect_object(const rect_object &r)
			: border(r.border.get_color()), fill(r.border.get_color()), border_width(r.border_width)
		{
			register_resource(&border);
			register_resource(&fill);

			left = r.left;
			top = r.top;
			right = r.right;
			bottom = r.bottom;
		}
		rect_object(const rect &r)
			: border(color(255, 255, 255)), fill(color(0, 0, 0, 0)), border_width(1.f)
		{
			register_resource(&border);
			register_resource(&fill);

			left = r.left;
			top = r.top;
			right = r.right;
			bottom = r.bottom;
		}
		~rect_object()
		{
			unregister_resource(&border);
			unregister_resource(&fill);
		}

		void set_border_color(color c)
		{
			border.set_color(c);
		}
		color get_border_color()
		{
			return border.get_color();
		}

		void set_fill_color(color c)
		{
			fill.set_color(c);
		}
		color get_fill_color()
		{
			return fill.get_color();
		}

		void set_border_width(float w)
		{
			border_width = w;
		}
		float get_border_width()
		{
			return border_width;
		}

		virtual void draw(const paint_params &pp) override
		{
			this->rect::fill(pp, fill);
			this->rect::draw(pp, border, border_width);
		}

		virtual void set_position(const point &p) override
		{
			float w = width(), h = height();
			left = p.x;
			top = p.y;
			right = left + w;
			bottom = top + h;
		}
		virtual void set_size(const size &s) override
		{
			right = left + s.width;
			bottom = top + s.height;
		}
		virtual point get_position() override
		{
			return {left, top};
		}
		virtual size get_size() override
		{
			return {width(), height()};
		}
	};

} }
