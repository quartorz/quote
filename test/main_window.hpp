#pragma once

#include "quote/win32/window.hpp"
#include "quote/win32/creator.hpp"
#include "quote/win32/procs.hpp"

#include "quote/direct2d/painter.hpp"
#include "quote/direct2d/integrated_manager.hpp"

#include "quote/direct2d/solid_brush.hpp"
#include "quote/direct2d/font.hpp"
#include "quote/direct2d/text.hpp"

namespace window_system = quote::win32;
namespace paint = quote::direct2d;

class main_window:
	public window_system::window<
		main_window,
		paint::painter<main_window>,
		window_system::quit_on_close<main_window>
	>,
	public window_system::creator<main_window>,
	public paint::integrated_manager<main_window>
{
	paint::solid_brush *brush;
	paint::font *font;
	paint::text *text;

public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}

	bool initialize()
	{
		this->init_resource(brush);
		this->init_resource(font);
		this->init_object(text);

		text->set_position(paint::point(30, 100));
		text->set_word_wrapping(paint::text::WordWrapping::NoWrap);
		text->set_text(L"QUOTE");

		return true;
	}

	void uninitialize()
	{
		this->delete_resource(brush);
		this->delete_resource(font);
		this->delete_object(text);
	}

	void draw(const paint_params &pp)
	{
		this->clear(pp, quote::direct2d::color(0, 0, 0, 0));
		paint::circle(paint::point(30, 30), 20).fill(pp, brush);
		this->draw_text(pp, font, brush, L"text", paint::rect(50, 60, 200, 300));
		this->paint::integrated_manager<main_window>::draw(pp);
	}
};
