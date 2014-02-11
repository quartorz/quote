#pragma once

#include "text.hpp"
#include "image.hpp"

namespace quote{ namespace direct2d{

	template <class Derived>
	class object_creator{
	public:
		text *create_text();
		image *create_image(const wchar_t *filename = nullptr);
	};

} }

#include "details/object_creator.hpp"
