#pragma once

#include <quote/button.hpp>

namespace quote{

	template <class Traits>
	class flat_button: public button<Traits>{
		typename Traits::solid_brush brush[3];
		typename Traits::text text;
		typename Traits::color color[3];

	protected:
		virtual void set_state(state) override;

	public:
		flat_button();

		void set_text(const wchar_t *);
		void set_text_color(state, typename Traits::color);
		void set_text_size(float);
		void set_color(state, typename Traits::color);

		virtual void set_position(const typename Traits::point &) override;
		virtual void set_size(const typename Traits::size &) override;

		virtual bool create_resource(const typename Traits::creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const typename Traits::paint_params &) override;
	};

}

#include <quote/impl/flat_button.hpp>
