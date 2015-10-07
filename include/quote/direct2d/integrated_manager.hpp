#pragma once

#include "../integrated_manager.hpp"
#include "traits.hpp"

namespace quote{
	namespace direct2d{

		template <class Derived>
		using integrated_manager = ::quote::integrated_manager<Derived, traits>;

	}
}
