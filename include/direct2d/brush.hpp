#pragma once

#include "base_types.hpp"
#include "resource.hpp"

#include "utils.hpp"

namespace quote{ namespace direct2d{

	class brush: public resource{
	protected:
		ID2D1Brush *instance;

	public:
		brush(): instance(nullptr)
		{
		}
		~brush()
		{
			::quote::utils::SafeRelease(instance);
		}

		ID2D1Brush *Get() const
		{
			return instance;
		}
	};

} }
