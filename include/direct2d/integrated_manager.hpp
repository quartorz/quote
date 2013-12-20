#pragma once

#include "../integrated_manager.hpp"
#include "traits.hpp"

namespace quote{
	namespace direct2d{

		template <class Derived, bool CheckDuplicate = false, bool MultiThread = false>
		using integrated_manager = ::quote::integrated_manager<Derived, traits, CheckDuplicate, MultiThread>;

	}
}
