#pragma once

#include <type_traits>

#include <quote/tmp/index_tuple.hpp>

namespace quote{ namespace tmp{

	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N, class = void>
	class make_filtered_index_range_impl;
	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N>
	using make_filtered_index_range = typename make_filtered_index_range_impl<Pred, Begin, N, void>::type;

	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N>
	class make_filtered_index_range_impl<
		Pred,
		Begin,
		N,
		typename std::enable_if<N == 0>::type
	> : public index_tuple<>
	{
	};

	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N>
	class make_filtered_index_range_impl<
		Pred,
		Begin,
		N,
		typename std::enable_if<N == 1 && Pred<Begin>::value>::type
	> : public index_tuple<Begin>
	{
	};

	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N>
	class make_filtered_index_range_impl<
		Pred,
		Begin,
		N,
		typename std::enable_if<N == 1 && !Pred<Begin>::value>::type
	> : public index_tuple<>
	{
	};

	template <template <std::size_t> class Pred, std::size_t Begin, std::size_t N>
	class make_filtered_index_range_impl<
		Pred,
		Begin,
		N,
		typename std::enable_if<(N > 1)>::type
	> : public concat<
		make_filtered_index_range<Pred, Begin, N / 2 + N % 2>,
		make_filtered_index_range<Pred, Begin + N / 2 + N % 2, N / 2>
	>
	{
	};

	template <template <std::size_t> class Pred, std::size_t N>
	using make_filtered_index = make_filtered_index_range<Pred, 0, N>;

} }
