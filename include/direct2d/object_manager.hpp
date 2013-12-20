#pragma once

#include "../object_manager.hpp"
#include "traits.hpp"

namespace quote{ namespace direct2d{

	template <class Derived, bool CheckDuplicate = false, bool MultiThread = false>
	using object_manager = ::quote::object_manager<Derived, traits, CheckDuplicate, MultiThread>;

} }
