#pragma once

#include <quote/resource_manager.hpp>
#include <quote/direct2d/resource_creator.hpp>
#include <quote/direct2d/traits.hpp>

namespace quote{ namespace direct2d{

	template <class Derived>
	class resource_manager:
		public ::quote::resource_manager<Derived, struct traits>,
		public resource_creator<resource_manager<Derived>>
	{
	};

} }
