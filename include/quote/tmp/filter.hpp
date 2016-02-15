#pragma once

#include <quote/tmp/append_if.hpp>

namespace quote{ namespace tmp{

	template <template <class Type> class Predication, class Result, class...>
	class filter_impl;

	template <template <class Type> class Predication, class... Result, class Head, class... Tail>
	class filter_impl<Predication, std::tuple<Result...>, Head, Tail...>{
	public:
		using type = typename filter_impl<
			Predication,
			typename append_if<
				Predication<Head>::value,
				Head,
				std::tuple<Result...>>::type,
			Tail...>::type;
	};

	template <template <class Type> class Predication, class... Result>
	class filter_impl<Predication, std::tuple<Result...>>{
	public:
		using type = std::tuple<Result...>;
	};

	template <template <class Type> class Predication, class... Types>
	using filter = typename filter_impl<Predication, std::tuple<>, Types...>::type;

} }
