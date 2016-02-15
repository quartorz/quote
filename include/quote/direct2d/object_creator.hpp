#pragma once

#include <quote/direct2d/text.hpp>
#include <quote/direct2d/image.hpp>

namespace quote{ namespace direct2d{

	template <class Derived>
	class object_creator{
	public:
		text *create_text();
		image *create_image(const wchar_t *filename = nullptr);
	};

} }

#include <quote/direct2d/impl/object_creator.hpp>
