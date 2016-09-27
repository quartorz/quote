#pragma once

#include <quote/tmp/tautology.hpp>

#include <cstddef>
#include <type_traits>

namespace quote{ namespace tmp{

	template <::std::size_t... I>
	class index_tuple{
	public:
		using type = index_tuple<I...>;
	};

	template <class Left, class Right>
	class concat_impl;
	template <::std::size_t... Left, ::std::size_t... Right>
	class concat_impl<index_tuple<Left...>, index_tuple<Right...>>
		: public index_tuple<Left..., Right...>
	{
	};
	template <class Left, class Right>
	using concat = typename concat_impl<Left, Right>::type;

	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t N, class = void>
	class make_filtered_index_range_impl;
	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t End>
	using make_filtered_index_range = typename make_filtered_index_range_impl<Filter, Begin, End - Begin, void>::type;

	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t N>
	class make_filtered_index_range_impl<
		Filter,
		Begin,
		N,
		typename ::std::enable_if<N == 0>::type
	> : public index_tuple<>
	{
	};

	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t N>
	class make_filtered_index_range_impl<
		Filter,
		Begin,
		N,
		typename ::std::enable_if<N == 1 && Filter<Begin>::value>::type
	> : public index_tuple<Begin>
	{
	};

	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t N>
	class make_filtered_index_range_impl<
		Filter,
		Begin,
		N,
		typename ::std::enable_if<N == 1 && !Filter<Begin>::value>::type
	> : public index_tuple<>
	{
	};

	template <template <::std::size_t> class Filter, ::std::size_t Begin, ::std::size_t N>
	class make_filtered_index_range_impl<
		Filter,
		Begin,
		N,
		typename ::std::enable_if<(N > 1)>::type
	> : public concat<
		typename make_filtered_index_range_impl<Filter, Begin, N / 2 + N % 2>::type,
		typename make_filtered_index_range_impl<Filter, Begin + N / 2 + N % 2, N / 2>::type
	>
	{
	};

	template <template <::std::size_t> class Filter, ::std::size_t N>
	using make_filtered_index = make_filtered_index_range<Filter, 0, N>;

	template <::std::size_t Begin, ::std::size_t End>
	using make_index_range = make_filtered_index_range<tautological_filter, Begin, End>;

	template <::std::size_t N>
	using make_index = make_index_range<0, N>;

} }
