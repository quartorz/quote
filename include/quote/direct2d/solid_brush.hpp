#pragma once

#include "brush.hpp"
#include "base_types.hpp"

namespace quote{ namespace direct2d{

	class solid_brush: public brush{
		color c;

	public:
		solid_brush(const color &c=color()): c(c)
		{
		}
		solid_brush(const solid_brush &b): c(b.c)
		{
		}

		void set_color(const color &);
		const color &get_color() const;
		color get_color();

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
	};

} }

#include "impl/solid_brush.hpp"
