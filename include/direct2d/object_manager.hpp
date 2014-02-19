#pragma once

#include "../object_manager.hpp"
#include "object_creator.hpp"
#include "traits.hpp"

namespace quote{ namespace direct2d{

	template <class Derived>
	class object_manager:
		public ::quote::object_manager<Derived, traits>,
		public object_creator<object_manager<Derived>>
	{
	};

} }
