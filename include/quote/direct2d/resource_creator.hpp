#pragma once

#include <quote/direct2d/font.hpp>
#include <quote/direct2d/solid_brush.hpp>

namespace quote{ namespace direct2d{

	template <class Derived>
	class resource_creator{
	public:
		font *create_font(const wchar_t *fontname = L"Meiryo");
		solid_brush *create_solid_brush(color c = color());
	};

} }

#include <quote/direct2d/impl/resource_creator.hpp>
