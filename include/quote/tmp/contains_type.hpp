#pragma once

#include <type_traits>
#include <tuple>

namespace quote{ namespace tmp{

	template <class, class...>
	class contains_type{
	public:
		static const bool value = false;
		using type = void;
	};

	template <class Type, class Head, class... Tail>
	class contains_type<Type, Head, Tail...>{
	public:
		using type = typename std::conditional<
			std::is_same<Type, Head>::value,
			Type,
			contains_type<Type, Tail...>>::type;
		static const bool value = typename std::conditional<
			std::is_same<Type, Head>::value,
			std::true_type,
			std::integral_constant<bool, contains_type<Type, Tail...>::value>>::type::value;
	};

	template <class Type, class... Types>
	class contains_type<Type, std::tuple<Types...>>
		: public contains_type<Type, Types...>
	{
	};

} }
