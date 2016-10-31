#pragma once

#include <quote/flat_button.hpp>

#include <functional>

namespace quote{

	template <class Traits>
	class callback_flat_button: public flat_button<Traits>{
		std::function<void()> callback;

	public:
		void on_push() override
		{
			callback();
		}

		void set_callback(std::function<void()> c)
		{
			callback = c;
		}
	};

}

#include <quote/impl/flat_button.hpp>
