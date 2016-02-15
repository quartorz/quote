#pragma once

#include <quote/direct2d/base_types.hpp>
#include <quote/direct2d/resource.hpp>
#include <quote/direct2d/object.hpp>
#include <quote/direct2d/solid_brush.hpp>
#include <quote/direct2d/font.hpp>
#include <quote/direct2d/text.hpp>
#include <quote/direct2d/userdefined_object.hpp>

namespace quote{ namespace direct2d{

	struct traits{
		using creation_params = creation_params;
		using paint_params = paint_params;
		using color = color;
		using point = point;
		using size = size;
		using rect = rect;
		using line = line;
		using circle = circle;
		using polygon = polygon;
		using resource = resource;
		using object = object;
		using brush = brush;
		using solid_brush = solid_brush;
		using font = font;
		using text = text;

		template <class Derived>
		using userdefined_object = userdefined_object<Derived>;
	};

} }
