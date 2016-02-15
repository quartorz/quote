#pragma once

#include <quote/direct2d/userdefined_object.hpp>
#include <quote/direct2d/solid_brush.hpp>
#include <quote/direct2d/text.hpp>
#include <quote/direct2d/flat_button.hpp>

namespace paint = quote::direct2d;

template <unsigned Button>
class dialog: public paint::userdefined_object<dialog<Button>>{
	using base = paint::userdefined_object<dialog<Button>>;

	class button: public paint::flat_button{
		std::function<void(void)> handler;

	public:
		button()
		{
			set_color(state::none, paint::color(0, 0, 0, 50));
			set_color(state::hover, paint::color(50, 50, 50, 20));
			set_color(state::push, paint::color(0, 0, 0, 80));
			set_text_color(state::none, paint::color(255, 255, 255, 200));
			set_text_color(state::hover, paint::color(255, 255, 255));
			set_text_color(state::push, paint::color(255, 255, 255, 150));
		}
		virtual void on_push() override
		{
			handler();
		}
		void set_handler(std::function<void(void)> f)
		{
			handler = f;
		}
	};

	paint::text text;
	paint::point origin, push_pos;
	paint::solid_brush border;

	std::array<button, Button> buttons;

	bool pushing = false, showing = false;

public:
	dialog()
	{
		register_object(&text);
		text.set_position(paint::point(10, 10));

		for(auto &b: buttons){
			register_object(&b);
		}

		register_resource(&border);
		border.set_color(paint::color());
	}
	~dialog()
	{
		unregister_object(&text);

		for(auto &b: buttons){
			unregister_object(&b);
		}

		unregister_resource(&border);
	}
	void show()
	{
		showing = true;
	}
	void hide()
	{
		showing = false;
	}
	void set_text(const wchar_t *t)
	{
		text.set_text(t);
	}
	void set_button_text(int i, const wchar_t *t)
	{
		buttons[i].set_text(t);
	}
	void set_button_handler(int i, std::function<void(void)> f)
	{
		buttons[i].set_handler(f);
	}
	virtual bool is_colliding(const paint::point &p) override
	{
		if(!showing)
			return false;
		return p.is_inside(get_rect());
	}
	virtual void on_left_press(const paint::point &p, hittest &ht) override
	{
		base::on_left_press(p, ht);

		if(!base::is_colliding(p) || text.is_colliding(p - get_position())){
			pushing = true;
			ht.set_cursor(hittest::cursor::hand);
			origin = get_position();
			push_pos = p;
		}
	}
	virtual void on_left_release(const paint::point &p, hittest &ht) override
	{
		pushing = false;
		base::on_left_release(p, ht);
	}
	virtual void on_mouse_move(const paint::point &p, hittest &ht) override
	{
		if(!pushing){
			base::on_mouse_move(p, ht);
			if(!base::is_colliding(p) || text.is_colliding(p - get_position()))
				ht.set_cursor(hittest::cursor::hand);
		}else{
			ht.set_cursor(hittest::cursor::hand);
			set_position(origin + p - push_pos);
		}
	}
	virtual void set_size(const paint::size &s) override
	{
		text.set_size({s.width - 2 * 10, s.height - 10 * 2 - 30});

		for(unsigned i = 0; i < Button; ++i){
			buttons[i].set_size({(s.width - 10 * (Button + 1)) / Button, 30});
			buttons[i].set_position({10 + (s.width - 10 * (Button - 1)) / Button * i, s.height - 40});
		}

		this->base::set_size(s);
	}
	virtual void draw(const paint::paint_params &pp) override
	{
		if(!showing)
			return;

		for(auto &b: buttons){
			pp.target->PushAxisAlignedClip(b.get_rect() + get_position(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pp.clear(paint::color(0, 0, 0, 0));
			pp.target->PopAxisAlignedClip();
		}

		this->base::draw(pp);

		get_rect().draw(pp, border);
	}
};

template <unsigned Button>
class modal_dialog: public paint::userdefined_object<modal_dialog<Button>>{
	using base = paint::userdefined_object<modal_dialog<Button>>;

	dialog<Button> dialog_;
	bool showing = false;

public:
	modal_dialog()
	{
		base::set_size({2000, 1000});

		register_object(&dialog_);
	}
	~modal_dialog()
	{
		unregister_object(&dialog_);
	}
	void show()
	{
		showing = true;
		dialog_.show();
	}
	void hide()
	{
		showing = false;
		dialog_.hide();
	}
	void set_text(const wchar_t *t)
	{
		dialog_.set_text(t);
	}
	void set_button_text(int i, const wchar_t *t)
	{
		dialog_.set_button_text(i, t);
	}
	void set_button_handler(int i, std::function<void(void)> f)
	{
		dialog_.set_button_handler(i, f);
	}
	virtual bool is_colliding(const paint::point &p) override
	{
		return showing;
	}
	virtual void set_size(const paint::size &s) override
	{
		dialog_.set_size(s);
	}
	virtual void set_position(const paint::point &p) override
	{
		dialog_.set_position(p);
	}
};
