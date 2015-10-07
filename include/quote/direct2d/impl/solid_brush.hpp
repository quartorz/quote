#pragma once

#include "../solid_brush.hpp"

#include "../utils.hpp"

namespace quote{ namespace direct2d{

	void solid_brush::set_color(const color &c)
	{
		this->c = c;

		if(instance != nullptr)
			static_cast<ID2D1SolidColorBrush*>(instance)->SetColor(c);
	}

	const color &solid_brush::get_color() const
	{
		return c;
	}

	color solid_brush::get_color()
	{
		return c;
	}

	bool solid_brush::create_resource(const creation_params &cp)
	{
		if(instance != nullptr)
			return true;
		return SUCCEEDED(cp.target->CreateSolidColorBrush(
			c,
			reinterpret_cast<ID2D1SolidColorBrush**>(&instance)));
	}

	void  solid_brush::destroy_resource()
	{
		::quote::utils::SafeRelease(instance);
	}

} }
