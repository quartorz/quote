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
#include "quote/direct2d/image.hpp"
#include "quote/direct2d/flat_button.hpp"

namespace window_system = quote::win32;
namespace paint = quote::direct2d;

#include "function.hpp"

static const int timer_id = 100;

#include <memory>

class main_window:
	public window_system::window<
		main_window,
		paint::painter<main_window>,
		window_system::resizer<main_window>,
		window_system::keyboard<main_window>,
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
		shooting,
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

	std::unique_ptr<title> title_;
	std::unique_ptr<configurator> config;
	std::unique_ptr<shooting> shooting_;

public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}

	main_window();

	bool initialize();
	void uninitialize();

	void select_scene(scene s)
	{
		this->paint::scene_manager<main_window>::select_scene(static_cast<int>(s));
	}
};

#include "title.hpp"
#include "configurator.hpp"
#include "shooting.hpp"

inline main_window::main_window():
	title_(std::make_unique<main_window::title>(this)),
	config(std::make_unique<main_window::configurator>(this)),
	shooting_(std::make_unique<main_window::shooting>(this))
{
}

inline bool main_window::initialize()
{
	add_scene(static_cast<int>(scene::title), title_.get());
	add_scene(static_cast<int>(scene::config), config.get());
	add_scene(static_cast<int>(scene::shooting), shooting_.get());

	select_scene(scene::title);

	return true;
}

inline void main_window::uninitialize()
{
	remove_scene(0);
	remove_scene(1);
}
