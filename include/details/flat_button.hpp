#pragma once

#include "../flat_button.hpp"

#include <functional>
#include <algorithm>

namespace quote{

	template <class Traits>
	void flat_button<Traits>::set_state(State s)
	{
		this->button<Traits>::set_state(s);
		text.set_color(color[static_cast<int>(s)]);
	}

	template <class Traits>
	flat_button<Traits>::flat_button()
	{
		text.set_align(Traits::text::Align::Center);
		text.set_paragraph_align(Traits::text::ParagraphAlign::Center);
	}

	template <class Traits>
	void flat_button<Traits>::set_text(const wchar_t *t)
	{
		text.set_text(t);
	}

	template <class Traits>
	void flat_button<Traits>::set_text_color(State s, typename Traits::color c)
	{
		color[static_cast<int>(s)] = c;
		set_state(state);
	}

	template <class Traits>
	void flat_button<Traits>::set_text_size(float s)
	{
		text.get_font().set_font_size(s);
	}

	template <class Traits>
	void flat_button<Traits>::set_color(State s, typename Traits::color c)
	{
		brush[static_cast<int>(s)].set_color(c);
	}

	template <class Traits>
	void flat_button<Traits>::set_position(const typename Traits::point &p)
	{
		this->button<Traits>::set_position(p);
		text.set_position(p);
	}

	template <class Traits>
	void flat_button<Traits>::set_size(const typename Traits::size &s)
	{
		this->button<Traits>::set_size(s);
		text.set_size(s);
	}

	template <class Traits>
	bool flat_button<Traits>::create_resource(const typename Traits::creation_params &cp)
	{
		return text.create_resource(cp)
			&& std::all_of(
				std::begin(brush), std::end(brush),
				std::bind(
					&Traits::solid_brush::create_resource,
					std::placeholders::_1,
					std::ref(cp)));
	}

	template <class Traits>
	void flat_button<Traits>::destroy_resource()
	{
		text.destroy_resource();
		std::for_each(std::begin(brush), std::end(brush), std::mem_fn(&Traits::solid_brush::destroy_resource));
	}

	template <class Traits>
	void flat_button<Traits>::draw(const typename Traits::paint_params &pp)
	{
		this->get_rect().fill(pp, brush[static_cast<int>(state)]);
		text.draw(pp);
	}

}
