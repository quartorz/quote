#pragma once

#include "main_window.hpp"

#include "keyconfig.hpp"

// キー(キーボードとジョイスティック)とボタン(ゲーム内のボタン)を関連付ける

class main_window::configurator: public paint::scene<>{
	main_window &window;

	class back_button: public main_window::button_base{
		main_window &window;

	public:
		back_button(main_window *w): window(*w)
		{
		}
		virtual void on_push() override
		{
			window.select_scene(main_window::scene::title);
		}
	};

	class key_assignor: public paint::userdefined_object<key_assignor>{
		using base = paint::userdefined_object<key_assignor>;

		configurator *config;

		paint::text button;
		button_base key;
		std::wstring key_str;
		int index;
		bool push = false;
		bool focus = false;

	public:
		key_assignor()
		{
			register_object(&button);
			button.set_rect(paint::rect(paint::point(10, 10), paint::size(100, 40)));
			button.set_alignment(paint::text::alignment::right);
			button.set_paragraph_align(paint::text::paragraph_align::center);

			register_object(&key);
			key.set_rect(paint::rect(paint::point(140, 10), paint::size(150, 40)));
		}
		~key_assignor()
		{
			unregister_object(&button);
			unregister_object(&key);
		}
		void init(configurator *c, const wchar_t *button_name, int index)
		{
			config = c;
			button.set_text(button_name);
			this->index = index;
		}
		bool is_colliding(const paint::point &p) override
		{
			return p.is_inside(get_rect());
		}
		void on_mouse_move(const paint::point &p, hittest &ht) override
		{
			ht.set_cursor(hittest::cursor::hand);
			if(focus)
				return;
			if(is_colliding(p)){
				if(push)
					key.set_state(button_base::state::push);
				else
					key.set_state(button_base::state::hover);
			}else
				key.set_state(button_base::state::hover);
		}
		void on_left_press(const paint::point &p, hittest &ht) override
		{
			push = true;
			key.set_state(button_base::state::push);
		}
		void on_left_release(const paint::point &p, hittest &ht) override
		{
			push = false;
			if(is_colliding(p)){
				focus = true;
				key.set_state(button_base::state::push);
				key.set_text(L"Push Key");
				config->select_assignor(index);
			}
		}
		void on_mouse_leave(const hittest &ht) override
		{
			if(!focus)
				key.set_state(button_base::state::none);
		}
		void on_lose_focus(const hittest &ht) override
		{
			focus = false;
			key.set_state(button_base::state::none);
			key.set_text(key_str.c_str());
			config->select_assignor(-1);
		}
		void draw(const paint::paint_params &pp) override
		{
			// deprecated
			pp.target->PushAxisAlignedClip(key.get_rect() + get_position(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pp.clear(paint::color(0, 0, 0, 0));
			pp.target->PopAxisAlignedClip();

			this->base::draw(pp);
		}
		void set_key_assignment(const wchar_t *k)
		{
			key_str = k;
			if(!focus)
				key.set_text(k);
		}
	};

	void select_assignor(int n)
	{
		assignor = n;
	}
	
	keyconfig *config = keyconfig::get_instance();
	std::vector<std::tuple<joystick_id, std::wstring>> joysticks;

	paint::text title, text[2];

	back_button back;

	std::array<key_assignor, keyconfig::button_count * 2> assignors;
	int assignor = -1;

	paint::text js_name;

public:
	configurator(main_window *w): window(*w), back(w)
	{
		// initialize objects
		register_object(&title);
		title.set_text(L"Key Config");
		title.set_size(paint::size(800, 90));
		title.set_alignment(paint::text::alignment::center);
		title.set_paragraph_align(paint::text::paragraph_align::center);
		title.get_font().set_size(30.f);

		register_object(&text[0]);
		text[0].set_text(L"Keyboard");
		text[0].set_rect({paint::point(0, 55), paint::size(400, 70)});
		text[0].set_alignment(paint::text::alignment::center);
		text[0].set_paragraph_align(paint::text::paragraph_align::center);
		text[0].get_font().set_size(25.f);

		register_object(&text[1]);
		text[1].set_text(L"Joystick");
		text[1].set_rect({paint::point(400, 55), paint::size(400, 70)});
		text[1].set_alignment(paint::text::alignment::center);
		text[1].set_paragraph_align(paint::text::paragraph_align::center);
		text[1].get_font().set_size(25.f);

		register_object(&js_name);
		js_name.set_rect({paint::point(490, 535), paint::size(310, 200)});

		register_object(&back);
		back.set_text(L"Back");
		back.set_rect({paint::point(25, 25), paint::size(80, 40)});

		static const wchar_t *button_names[] ={
			L"Up",
			L"Down",
			L"Left",
			L"Right",
			L"A",
			L"B",
			L"C",
			L"Menu",
		};
		for(unsigned i = 0; i < keyconfig::button_count * 2; ++i){
			assignors[i].init(this, button_names[i % 8], i);
			register_object(&assignors[i]);
			assignors[i].set_size({300, 60});
			if(i < 8){
				assignors[i].set_position({50, 110 + 50.f * i});
			} else{
				assignors[i].set_position({450, 110 + 50.f * (i % 8)});
			}
		}

		// initialize handlers
		add_timer_handler([this](unsigned){
			this->window.repaint();
		}, timer_id);

		add_keyboard_handler([this](unsigned keycode, bool push){
			if(push && 0 <= assignor && assignor < 8){
				auto str = config->set_keyboard_config(static_cast<keyconfig::button>(assignor), keycode);
				assignors[assignor].set_key_assignment(str);
				assignors[assignor].on_lose_focus(paint::object::create_hittest(&this->window));
				assignor = -1;
			}
		}, keycode_range(0, 255));
	}
	~configurator()
	{
		unregister_object(&title);
		unregister_object(&text[0]);
		unregister_object(&text[1]);
		unregister_object(&js_name);
		unregister_object(&back);

		for(auto &a: assignors){
			unregister_object(&a);
		}
	}
	void on_show() override
	{
		for(unsigned i = 0; i < 16; ++i){
			if(i < 8)
				assignors[i].set_key_assignment(config->get_keyboard_config(static_cast<keyconfig::button>(i)));
			else
				assignors[i].set_key_assignment(config->get_joystick_config(static_cast<keyconfig::button>(i - 8)));
		}

		config->reset();
	}
	void on_get_joystick_state(const joystick_id &id, const joystick_state &js) override
	{
		if(assignor >= 8){
			auto str = config->set_joystick_config(static_cast<keyconfig::button>(assignor - 8), id, js);
			if(std::wcsncmp(str, L"None", 5)){
				assignors[assignor].set_key_assignment(str);
				assignors[assignor].on_lose_focus(paint::object::create_hittest(&window));
				assignor = -1;

				auto it = std::find_if(joysticks.begin(), joysticks.end(), [&id](std::tuple<joystick_id, std::wstring> &t){return std::get<0>(t) == id; });
				if(js_name.get_text() != std::get<1>(*it)){
					js_name.set_text(std::get<1>(*it).c_str());
				}
			}
		}
	}
	void on_reload_joysticks(const std::vector<std::tuple<joystick_id, std::wstring>> &jss) override
	{
		joysticks = jss;

		auto &id = config->get_joystick_id();
		auto it = std::find_if(joysticks.begin(), joysticks.end(), [&id](std::tuple<joystick_id, std::wstring> &t){return std::get<0>(t) == id; });
		if(it != joysticks.end()){
			js_name.set_text(std::get<1>(*it).c_str());
		}
	}
	void draw(const paint::paint_params &pp) override
	{
		// deprecated
		pp.target->PushAxisAlignedClip(back.get_rect(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pp.clear(paint::color(0, 0, 0, 0));
		pp.target->PopAxisAlignedClip();

		this->paint::scene<>::draw(pp);
	}
};
