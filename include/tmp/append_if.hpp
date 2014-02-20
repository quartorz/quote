#pragma once

#include <tuple>

namespace quote{ namespace tmp{

	template <bool Condition, class Type, class Tuple>
	class append_if;

	template <class Type, class... Types>
	class append_if<true, Type, std::tuple<Types...>>{
	public:
		using type = std::tuple<Types..., Type>;
	};

	template <class Type, class... Types>
	class append_if<false, Type, std::tuple<Types...>>{
	public:
		using type = std::tuple<Types...>;
	};

} }
