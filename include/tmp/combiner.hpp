#pragma once

#include "index_tuple.hpp"
#include "nil.hpp"

#include <tuple>
#include <type_traits>

namespace quote{ namespace tmp{

	template <class... Functions>
	class combiner{

		template <class Function, class... Args>
		class result{
			using result_type = typename std::result_of<Function(Args...)>::type;

		public:
			using type = typename std::conditional<std::is_void<result_type>::value, nil, result_type>::type;
		};

		template <class... Args>
		using get_result_tuple = std::tuple<typename result<Functions, Args...>::type...>;

		using function_tuple = std::tuple<Functions...>;

		function_tuple functions;

	public:
		combiner(Functions... functions): functions(functions...)
		{
		}

		template <class... Args>
		get_result_tuple<Args...> operator()(Args... args)
		{
			return operator_helper(make_index<sizeof...(Functions)>(), args...);
		}

	private:
		template <std::size_t... Indices, class... Args>
		get_result_tuple<Args...> operator_helper(index_tuple<Indices...>, Args... args)
		{
			return get_result_tuple<Args...>(bind<Indices>(args...)...);
		}

		template <std::size_t I, class... Args>
		typename std::tuple_element<I, get_result_tuple<Args...>>::type bind(Args... args)
		{
			using result_type = typename std::tuple_element<I, get_result_tuple<Args...>>::type;
			return bind_helper<result_type>(std::get<I>(functions), args...);
		}

		template <class Result, class Function, class... Args>
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

		template <class Function, class... Args>
		class bind_helper_class<nil, Function, Args...>{
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

		template <class Result, class Function, class... Args>
		bind_helper_class<Result, Function, Args...> bind_helper(Function &f, Args... args)
		{
			return bind_helper_class<Result, Function, Args...>(f, args...);
		}
	};

	template <class... Functions>
	combiner<Functions...> make_combiner(Functions... functions)
	{
		return combiner<Functions...>(functions...);
	}

} }
