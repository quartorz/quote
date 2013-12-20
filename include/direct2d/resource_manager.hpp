#pragma once

#include "../resource_manager.hpp"
#include "traits.hpp"

namespace quote{ namespace direct2d{

	template <class Derived, bool CheckDuplicate = false, bool MultiThread = false>
	using resource_manager = ::quote::resource_manager<Derived, traits, CheckDuplicate, MultiThread>;

} }
