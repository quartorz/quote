#pragma once

#include <type_traits>

namespace quote{ namespace tmp{

	template <typename T>
	class print {
		static constexpr char value = 256;

	public:
		using type = T;
	};

	template <typename T>
	using print_t = typename print<T>::type;

} }
