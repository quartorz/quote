#pragma once

#include <quote/direct2d/scene.hpp>
#include <quote/direct2d/vertical_splitter.hpp>

namespace quote{ namespace direct2d{

	template <class... Options>
	class vertical_split_scene: public scene<Options...>{
		using base = scene<Options...>;

		vertical_splitter<Options...> splitter;

	public:
		vertical_split_scene()
		{
			this->register_object(&splitter);
		}
		~vertical_split_scene()
		{
			this->unregister_object(&splitter);
		}
		void set_left(object *o)
		{
			splitter.set_left(o);
		}
		void set_right(object *o)
		{
			splitter.set_right(o);
		}
		object *get_left()
		{
			return splitter.get_left();
		}
		object *get_right()
		{
			return splitter.get_right();
		}
		void set_bar_color(color c)
		{
			splitter.set_color(c);
		}
		void set_bar_position(int pos)
		{
			splitter.set_bar_position(pos);
		}
		int get_bar_position()
		{
			return splitter.get_bar_position();
		}
		void set_position(const direct2d::point &p) override
		{
			splitter.set_position(p);
			this->base::set_position(p);
		}
		void set_size(const direct2d::size &s) override
		{
			splitter.set_size(s);
			this->base::set_size(s);
		}
	};

} }
