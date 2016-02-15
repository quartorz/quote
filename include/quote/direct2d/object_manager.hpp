#pragma once

#include <quote/object_manager.hpp>
#include <quote/direct2d/object_creator.hpp>
#include <quote/direct2d/traits.hpp>

namespace quote{ namespace direct2d{

	template <class Derived>
	class object_manager:
		public ::quote::object_manager<Derived, traits>,
		public object_creator<object_manager<Derived>>
	{
	};

} }
