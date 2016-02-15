#pragma once

#include <quote/direct2d/base_types.hpp>
#include <quote/direct2d/resource.hpp>

#include <quote/direct2d/utils.hpp>

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
