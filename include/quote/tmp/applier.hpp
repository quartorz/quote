#pragma once

#include <quote/tmp/index_tuple.hpp>
#include <quote/tmp/nil.hpp>

#include <tuple>
#include <type_traits>

namespace quote{ namespace tmp{

	template <class... Types>
	class applier{

		template <class Type, class Function, class... Args>
		class result{
			using result_type = typename ::std::result_of<Function(Type, Args...)>::type;

		public:
			using type = typename ::std::conditional<
				::std::is_void<result_type>::value,
				nil,
				result_type>::type;
		};
		template <class Function, class... Args>
		using result_tuple = ::std::tuple<typename result<Types, Function, Args...>::type...>;

	public:
		using tuple_type = ::std::tuple<Types...>;

	private:
		tuple_type tuple;

	public:
		applier()
		{
		}

		template <class = typename ::std::enable_if<sizeof...(Types) != 0>::type>
		applier(Types... types): tuple(types...)
		{
		}

		applier(::std::tuple<Types...> types): tuple(types)
		{
		}

		tuple_type &get()
		{
			return tuple;
		}

		const tuple_type &get() const
		{
			return tuple;
		}

		template <class Function, class... Args>
		result_tuple<Function, Args...> operator()(Function func, Args... args)
		{
			return call(make_index<sizeof...(Types)>(), func, args...);
		}

		template <::std::size_t... Indices, class Function, class... Args>
		result_tuple<Function, Args...> call(index_tuple<Indices...>, Function func, Args... args)
		{
			return result_tuple<Function, Args...>(bind<Indices>(func, args...)...);
		}

	private:
		template <::std::size_t I, class Function, class... Args>
		typename ::std::tuple_element<I, result_tuple<Function, Args...>>::type bind(Function func, Args... args)
		{
			using result_type = typename ::std::tuple_element<I, result_tuple<Function, Args...>>::type;
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

		template <class Result, ::std::size_t I, class Function, class... Args>
		Result bind_helper(Function func, Args... args)
		{
			return bind_helper_class<
				Result,
				typename ::std::tuple_element<I, tuple_type>::type,
				Function,
				Args...
			>(
				::std::get<I>(tuple),
				func,
				args...
			);
		}
	};

	template <class Tuple>
	class tuple_to_applier;
	template <class... Types>
	class tuple_to_applier<::std::tuple<Types...>>{
	public:
		using type = applier<Types...>;
	};

	template <class... Types>
	applier<Types...> make_applier(Types... objects)
	{
		return applier<Types...>(objects...);
	}

	template <class... Types>
	applier<Types...> make_applier(::std::tuple<Types...> objects)
	{
		return applier<Types...>(objects);
	}

} }
