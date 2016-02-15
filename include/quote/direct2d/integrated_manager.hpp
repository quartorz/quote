#pragma once

#include <quote/integrated_manager.hpp>
#include <quote/direct2d/traits.hpp>

namespace quote{
	namespace direct2d{

		template <class Derived>
		using integrated_manager = ::quote::integrated_manager<Derived, traits>;

	}
}
