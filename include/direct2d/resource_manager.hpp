#pragma once

#include "../resource_manager.hpp"
#include "resource_creator.hpp"
#include "traits.hpp"

namespace quote{ namespace direct2d{

	template <class Derived, bool CheckDuplicate = false, bool MultiThread = false>
	class resource_manager:
		public ::quote::resource_manager<Derived, traits, CheckDuplicate, MultiThread>,
		public resource_creator<resource_manager<Derived, CheckDuplicate, MultiThread>>
	{
	};

} }
