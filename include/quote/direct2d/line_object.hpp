#pragma once

#include <quote/direct2d/base_types.hpp>
#include <quote/direct2d/userdefined_object.hpp>

#include <quote/direct2d/solid_brush.hpp>

namespace quote{ namespace direct2d{

	class line_object: public userdefined_object<line_object>, public line{
		solid_brush border;
		float border_width;

	public:
		line_object()
			: border(color(255, 255, 255)), border_width(1.f)
		{
			register_resource(&border);
		}
		line_object(const line_object &l)
			: border(r.border.get_color()), border_width(r.border_width)
		{
			register_resource(&border);

			p1 = l.p1;
			p2 = l.p2;
		}
		line_object(const line &l)
			: border(color(255, 255, 255)), border_width(1.f)
		{
			register_resource(&border);

			p1 = l.p1;
			p2 = l.p2;
		}

		void set_border_color(color c)
		{
			border.set_color(c);
		}
		color get_border_color()
		{
			return border.get_color();
		}

		void set_border_width(float w)
		{
			border_width = w;
		}
		float get_border_width()
		{
			return border_width;
		}

		virtual void draw(const paint_params &pp)
		{
			this->line::draw(pp, border, border_width);
		}

		// TODO: ¡‚Íg‚í‚È‚¢‚©‚ç‹C‚ªŒü‚¢‚½‚ç‘‚­
		/*virtual void set_position(const point &p)
		{
		}
		virtual void set_size(const size &s)
		{
		}
		virtual point get_position()
		{
		}
		virtual size get_size()
		{
		}*/
	};

} }
