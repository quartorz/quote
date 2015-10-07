#pragma once

#include "../object_creator.hpp"

namespace quote{ namespace direct2d{

	template <class Derived>
	text *object_creator<Derived>::create_text()
	{
		auto t = new text;
		static_cast<Derived>(this)->register_object(t);
		return t;
	}

	template <class Derived>
	image *object_creator<Derived>::create_image(const wchar_t *filename /* =nullptr */)
	{
		auto i = new image(filename);
		static_cast<Derived>(this)->register_object(i);
		return i;
	}

} }
