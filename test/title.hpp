#pragma once

#include "main_window.hpp"

#include "dialog.hpp"

class main_window::title: public paint::scene<false>{
	main_window &window;

	class button: public button_base{
		title *t;
		int index;

	public:
		void init(title *t, int index)
		{
			this->t = t;
			this->index = index;
		}
		virtual void on_push() override
		{
			t->window.select_scene(static_cast<main_window::scene>(index));
		}
	};

	std::array<button, 2> buttons;

	modal_dialog<2> dialog;

public:
	title(main_window *w): window(*w)
	{
		static const wchar_t *button_labels[] ={
			L"Key Config",
			L"Shooting",
		};
		for(unsigned i = 0; i < buttons.size(); ++i){
			buttons[i].init(this, i + 1);
			register_object(&buttons[i]);
			buttons[i].set_text(button_labels[i]);
			buttons[i].set_rect({paint::point(0, 75.f * i), paint::size(150, 50)});
		}

		register_object(&dialog);
		dialog.set_size({200, 100});
		dialog.show();
		dialog.set_button_handler(0, [this]{this->dialog.hide();});

		add_timer_handler([this](unsigned){
			this->window.repaint();
		}, timer_id);
	}
	~title()
	{
		for(auto &b: buttons){
			unregister_object(&b);
		}

		unregister_object(&dialog);
	}
	virtual void draw(const paint::paint_params &pp) override
	{
		for(auto &b: buttons){
			pp.target->PushAxisAlignedClip(b.get_rect(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pp.clear(paint::color(0, 0, 0, 0));
			pp.target->PopAxisAlignedClip();
		}

		this->paint::scene<false>::draw(pp);
	}
};
