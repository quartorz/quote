#pragma once

#include "base_types.hpp"
#include "resource.hpp"
#include "object.hpp"
#include "solid_brush.hpp"
#include "font.hpp"
#include "text.hpp"

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
	};

} }
