#pragma once

#include "index_tuple.hpp"
#include "nil.hpp"

#include <tuple>
#include <type_traits>

namespace quote{ namespace tmp{

	template <class... Types>
	class applier{

		template <class Type, class Function, class... Args>
		class result{
			using result_type = typename std::result_of<Function(Type, Args...)>::type;

		public:
			using type = typename std::conditional<
				std::is_void<result_type>::value,
				nil,
				result_type>::type;
		};
		template <class Function, class... Args>
		using get_result_tuple = std::tuple<typename result<Types, Function, Args...>::type...>;

		using tuple_type = std::tuple<Types...>;

		tuple_type tuple;

	public:
		applier(Types... types): tuple(types...)
		{
		}

		applier(std::tuple<Types...> types): tuple(types)
		{
		}

		template <class Function, class... Args>
		get_result_tuple<Function, Args...> operator()(Function func, Args... args)
		{
			return operator_helper(make_index<sizeof...(Types)>(), func, args...);
		}

	private:
		template <std::size_t... Indices, class Function, class... Args>
		get_result_tuple<Function, Args...> operator_helper(index_tuple<Indices...>, Function func, Args... args)
		{
			return get_result_tuple<Function, Args...>(bind<Indices>(func, args...)...);
		}

		template <std::size_t I, class Function, class... Args>
		typename std::tuple_element<I, get_result_tuple<Function, Args...>>::type bind(Function func, Args... args)
		{
			using result_type = typename std::tuple_element<I, get_result_tuple<Function, Args...>>::type;
			return bind_helper<result_type, I>(func, args...);
		}

		template <class Result, class Element, class Function, class... Args>
		class bind_helper_class{
			Result result;

		public:
			bind_helper_class(Element &element, Function func, Args... args): result(func(element, args...))
			{
			}
			operator Result()
			{
				return result;
			}
		};

		template <class Element, class Function, class... Args>
		class bind_helper_class<nil, Element, Function, Args...>{
		public:
			bind_helper_class(Element &element, Function func, Args... args)
			{
				func(element, args...);
			}
			operator nil()
			{
				return nil();
			}
		};

		template <class Result, std::size_t I, class Function, class... Args>
		Result bind_helper(Function func, Args... args)
		{
			return bind_helper_class<
				Result,
				typename std::tuple_element<I, tuple_type>::type,
				Function,
				Args...
			>(
				std::get<I>(tuple),
				func,
				args...
			);
		}
	};

	template <class... Types>
	applier<Types...> make_applier(Types... objects)
	{
		return applier<Types...>(objects...);
	}

	template <class... Types>
	applier<Types...> make_applier(std::tuple<Types...> objects)
	{
		return applier<Types...>(objects);
	}

} }
