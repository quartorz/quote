#pragma once

#include <cstddef>

namespace quote{ namespace tmp{

	template <std::size_t... I>
	class index_tuple{
	public:
		using type = index_tuple<I...>;
	};

	template <class Left, class Right>
	class concat_impl;
	template <std::size_t... Left, std::size_t... Right>
	class concat_impl<index_tuple<Left...>, index_tuple<Right...>>
		: public index_tuple<Left..., Right...>
	{
	};
	template <class Left, class Right>
	using concat = typename concat_impl<Left, Right>::type;

	template <class Sequence, std::size_t N>
	class shift_impl;
	template <std::size_t N, std::size_t... I>
	class shift_impl<index_tuple<I...>, N>
		: public index_tuple<(I + N)...>
	{
	};
	template <class Sequence, std::size_t N>
	using shift = typename shift_impl<Sequence, N>::type;

	template <std::size_t Begin, std::size_t End>
	class make_index_range_impl;
	template <std::size_t Begin, std::size_t End>
	using make_index_range = typename make_index_range_impl<Begin, End>::type;

	template <std::size_t Begin, std::size_t End>
	class make_index_range_impl:
		public shift< concat<
			make_index_range<0, (End - Begin) / 2>,
			shift<make_index_range<0, (End - Begin) / 2 + (End - Begin) % 2>, (End - Begin) / 2>
		>, Begin>
	{
	};

	template <>
	class make_index_range_impl<0, 1>: public index_tuple<0>
	{
	};
	template <>
	class make_index_range_impl<0, 0>: public index_tuple<>
	{
	};

	template <std::size_t N>
	using make_index = make_index_range<0, N>;

} }
