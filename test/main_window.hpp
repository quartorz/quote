#pragma once

#include "quote/win32/window.hpp"
#include "quote/win32/creator.hpp"
#include "quote/win32/procs.hpp"
#include "quote/win32/object_processor.hpp"
#include "quote/win32/joystick.hpp"

#include "quote/direct2d/painter.hpp"
#include "quote/direct2d/scene_manager.hpp"

#include "quote/direct2d/scene.hpp"
#include "quote/direct2d/solid_brush.hpp"
#include "quote/direct2d/font.hpp"
#include "quote/direct2d/text.hpp"
#include "quote/direct2d/flat_button.hpp"

namespace window_system = quote::win32;
namespace paint = quote::direct2d;

static const int timer_id = 100;

class main_window:
	public window_system::window<
		main_window,
		paint::painter<main_window>,
		window_system::resizer<main_window>,
		window_system::keyboard<main_window, true>,
		window_system::joystick<main_window, timer_id>,
		window_system::object_processor<main_window, paint::traits>,
		window_system::quit_on_close<main_window>,
		window_system::timer<main_window>
	>,
	public window_system::creator<main_window>,
	public paint::scene_manager<main_window, false>
{
	enum class scene: int{
		title,
		config,
	};

	class button_base: public paint::flat_button{
	public:
		button_base()
		{
			set_color(state::none, paint::color(0, 0, 0, 50));
			set_color(state::hover, paint::color(50, 50, 50, 20));
			set_color(state::push, paint::color(0, 0, 0, 80));
			set_text_color(state::none, paint::color(255, 255, 255, 200));
			set_text_color(state::hover, paint::color(255, 255, 255));
			set_text_color(state::push, paint::color(255, 255, 255, 150));
		}
		virtual void set_state(state s) override
		{
			this->paint::flat_button::set_state(s);
		}
		virtual void on_push() override
		{
		}
	};

	class title;
	class configurator;
	class shooting;

	title *title_;
	configurator *config;

public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}
	bool initialize();
	void uninitialize();

	void select_scene(scene s)
	{
		this->paint::scene_manager<main_window>::select_scene(static_cast<int>(s));
	}
};

#include "title.hpp"
#include "configurator.hpp"

inline bool main_window::initialize()
{
	title_ = new title(this);
	add_scene(static_cast<int>(scene::title), title_);

	config = new configurator(this);
	add_scene(static_cast<int>(scene::config), config);

	select_scene(scene::title);

	return true;
}

inline void main_window::uninitialize()
{
	remove_scene(0);
	remove_scene(1);

	delete title_;
	delete config;
}
