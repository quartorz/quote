#pragma once

#include <quote/direct2d/resource_creator.hpp>

namespace quote{ namespace direct2d{

	template <class Derived>
	font *resource_creator<Derived>::create_font(const wchar_t *fontname /* =L"Meiryo" */)
	{
		auto f = new font;
		static_cast<Derived*>(this)->register_resource(f);
		f->set_name(fontname);
		return f;
	}

	template <class Derived>
	solid_brush *resource_creator<Derived>::create_solid_brush(color c /* =color() */ )
	{
		auto b = new solid_brush(c);
		static_cast<Derived*>(this)->register_resource(b);
		return b;
	}

} }
