#pragma once

#include "quote/win32/window.hpp"
#include "quote/win32/creator.hpp"
#include "quote/win32/procs.hpp"
#include "quote/win32/object_processor.hpp"

#include "quote/direct2d/painter.hpp"
#include "quote/direct2d/integrated_manager.hpp"

#include "quote/direct2d/solid_brush.hpp"
#include "quote/direct2d/font.hpp"
#include "quote/direct2d/text.hpp"
#include "quote/direct2d/flat_button.hpp"

namespace window_system = quote::win32;
namespace paint = quote::direct2d;

class button: public paint::flat_button{
	int n;

public:
	button(int n): n(n)
	{
	}

	void on_push()
	{
		static const wchar_t *s[] = {L"button0", L"button1"};
		::MessageBoxW(nullptr, L"push", s[n], 0);
	}
};

class main_window:
	public window_system::window<
		main_window,
		paint::painter<main_window>,
		window_system::quit_on_close<main_window>,
		window_system::object_processor<main_window, paint::traits>
	>,
	public window_system::creator<main_window>,
	public paint::integrated_manager<main_window>
{
	paint::solid_brush *brush;
	paint::font *font;
	paint::text *text;
	::button button{0}, button2{1};

public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}

	bool initialize()
	{
		init_resource(brush);
		init_resource(font);
		init_object(text);
		register_object(&button);
		register_object(&button2);

		text->set_position(paint::point(30, 100));
		text->set_word_wrapping(paint::text::WordWrapping::NoWrap);
		text->set_text(L"QUOTE");

		button.set_position(paint::point(100, 50));
		button.set_size(paint::size(100, 50));
		button.set_text(L"Button");
		button.set_color(::button::State::None, paint::color(128, 128, 128));
		button.set_color(::button::State::Hover, paint::color(255, 255, 255));
		button.set_color(::button::State::Push, paint::color(128, 128, 128));

		button2.set_position(paint::point(120, 70));
		button2.set_size(paint::size(100, 50));
		button2.set_text(L"Button");
		button2.set_color(::button::State::None, paint::color(128, 128, 128));
		button2.set_color(::button::State::Hover, paint::color(255, 255, 255));
		button2.set_color(::button::State::Push, paint::color(128, 128, 128));

		return true;
	}

	void uninitialize()
	{
		delete_resource(brush);
		delete_resource(font);
		delete_object(text);
		unregister_object(&button);
		unregister_object(&button2);
	}

	void draw(const paint_params &pp)
	{
		clear(pp, quote::direct2d::color(0, 0, 0, 0));
		paint::circle(paint::point(30, 30), 20).fill(pp, brush);
		draw_text(pp, font, brush, L"text", paint::rect(50, 60, 200, 300));

		this->paint::integrated_manager<main_window>::draw(pp);
	}
};
