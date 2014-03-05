#pragma once

#include "base_types.hpp"
#include "object.hpp"
#include "userdefined_object.hpp"

#include "../tmp/has_tag.hpp"
#include "../tmp/filter.hpp"
#include "../tmp/applier.hpp"

namespace quote{ namespace direct2d{

	namespace vertical_splitter_tag{

		struct on_set_bar_positon{};

	}

	namespace vertical_splitter_option{

		struct resize_on_set_bar_position{
			using tag = ::quote::direct2d::vertical_splitter_tag::on_set_bar_positon;

			template <class Splitter>
			void on_set_bar_positon(Splitter &splitter, const size &s, int pos)
			{
				splitter.get_left()->set_size({
					static_cast<float>(pos) - 1,
					s.height});
				splitter.get_right()->set_rect({
					point(static_cast<float>(pos)+1, 0),
					size(s.width - (static_cast<float>(pos)+1), s.height)});
			}
		};

	}

	template <class... Options>
	class vertical_splitter: public userdefined_object<vertical_splitter<Options...>>{
		using base = userdefined_object<vertical_splitter<Options...>>;

		struct on_set_bar_positon_binder{
			template <class Option, class Splitter>
			void operator()(Option &o, std::reference_wrapper<Splitter> &splitter, const size &s, int pos)
			{
				o.on_set_bar_positon(splitter.get(), s, pos);
			}
		};

		using on_set_bar_position_tuple = ::quote::tmp::filter<
			::quote::tmp::has_tag<vertical_splitter_tag::on_set_bar_positon>::type,
			Options...>;
		decltype(::quote::tmp::make_applier(on_set_bar_position_tuple())) on_set_bar_position;

		color c = color(0, 0, 0, 0);
		object *left = nullptr, *right = nullptr;
		int bar_pos = 0;
		bool pushing = false;

		rect get_bar_rect()
		{
			auto p = this->get_position();
			auto s = this->get_size();
			return direct2d::rect(direct2d::point(p.x + static_cast<float>(bar_pos) - 1.f, 0.f), direct2d::size(2.f, s.height));
		}

	public:
		void set_left(object *o)
		{
			if(left != nullptr)
				this->unregister_object(left);
			left = o;
			if(left != nullptr)
				this->register_object(left);
		}
		object *get_left()
		{
			return left;
		}
		void set_right(object *o)
		{
			if(right != nullptr)
				this->unregister_object(right);
			right = o;
			if(right != nullptr)
				this->register_object(right);
		}
		object *get_right()
		{
			return right;
		}
		void set_color(color c)
		{
			this->c = c;
		}
		void set_bar_position(int pos)
		{
			if(bar_pos != pos){
				auto s = this->get_size();
				if(0 < pos && pos < s.width){
					bar_pos = pos;
					on_set_bar_position(on_set_bar_positon_binder(), std::ref(*this), s, pos);
				}
			}
		}
		int get_bar_position()
		{
			return bar_pos;
		}
		void draw(const direct2d::paint_params &pp) override
		{
			this->base::draw(pp);

			auto p = this->get_position();
			auto r = get_bar_rect() + p;
			r.left++;

			pp.target->PushAxisAlignedClip(
				r,
				D2D1_ANTIALIAS_MODE_ALIASED);
			pp.target->Clear(c);
			pp.target->PopAxisAlignedClip();
		}
		bool is_colliding(const point &ap) override
		{
			return ap.is_inside(this->get_drawing_rect());
		}
		void on_mouse_move(const point &ap, hittest &ht) override
		{
			if(pushing){
				ht.set_cursor(hittest::cursor::leftright);
				this->set_bar_position(static_cast<int>(ap.x));
			}else if(!ap.is_inside(get_bar_rect()))
				this->base::on_mouse_move(ap, ht);
			else{
				ht.set_cursor(hittest::cursor::leftright);
			}
		}
		void on_left_press(const point &ap, hittest &ht) override
		{
			if(!ap.is_inside(get_bar_rect()))
				this->base::on_left_press(ap, ht);
			else{
				ht.set_cursor(hittest::cursor::leftright);
				pushing = true;
			}
		}
		void on_left_release(const point &ap, hittest &ht) override
		{
			pushing = false;

			if(!ap.is_inside(get_bar_rect()))
				this->base::on_left_release(ap, ht);
			else{
				ht.set_cursor(hittest::cursor::leftright);
			}
		}
	};

} }
