#pragma once

#include "index_tuple.hpp"
#include "nil.hpp"

#include <tuple>
#include <type_traits>

namespace quote{ namespace tmp{

	template <class ArgsTuple, class FunctionsTuple>
	class combiner;
	template <class... Args, class... Functions>
	class combiner<std::tuple<Args...>, std::tuple<Functions...>>{

		template <class Function, class ArgsTuple>
		class result;
		template <class Function, class... Args>
		class result<Function, std::tuple<Args...>>{
			using result_type = typename std::result_of<Function(Args...)>::type;

		public:
			using type = std::conditional_t<std::is_void<result_type>::value, nil, result_type>;
		};

		using function_tuple = std::tuple<Functions...>;
		using arg_tuple = std::tuple<Args...>;
		using result_tuple = std::tuple<typename result<Functions, arg_tuple>::type...>;

		function_tuple functions;
		result_tuple a;

	public:
		combiner(Functions... functions): functions(functions...)
		{
		}

		result_tuple operator()(Args... args)
		{
			return operator_helper(make_index<sizeof...(Functions)>(), args...);
		}

	private:
		template <std::size_t... Indices>
		result_tuple operator_helper(index_tuple<Indices...>, Args... args)
		{
			return result_tuple(bind<Indices>(args...)...);
		}

		template <std::size_t I>
		typename std::tuple_element<I, result_tuple>::type bind(Args... args)
		{
			using result_type = typename std::tuple_element<I, result_tuple>::type;
			return bind_helper<result_type>(std::get<I>(functions), args...);
		}

		template <class Result, class Function>
		class bind_helper_class{
			Result result;

		public:
			bind_helper_class(Function &f, Args... args): result(f(args...))
			{
			}
			operator Result()
			{
				return result;
			}
		};

		template <class Function>
		class bind_helper_class<nil, Function>{
		public:
			bind_helper_class(Function &f, Args... args)
			{
				f(args...);
			}
			operator nil()
			{
				return nil();
			}
		};

		template <class Result, class Function>
		bind_helper_class<Result, Function> bind_helper(Function &f, Args... args)
		{
			return bind_helper_class<Result, Function>(f, args...);
		}
	};

	template <class... Args, class... Functions>
	combiner<std::tuple<Args...>, std::tuple<Functions...>> make_combiner(Functions... functions)
	{
		return combiner<std::tuple<Args...>, std::tuple<Functions...>>(functions...);
	}

} }
