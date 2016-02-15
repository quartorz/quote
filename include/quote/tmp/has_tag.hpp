#pragma once

#include <quote/tmp/contains_type.hpp>

namespace quote{ namespace tmp{

	template <class Tag>
	class has_tag{
	public:
		template <class Type>
		using type = contains_type<Tag, typename Type::tag>;
	};

} }
