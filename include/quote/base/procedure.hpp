#pragma once

#include <quote/tmp/tautology.hpp>
#include <quote/tmp/index_tuple.hpp>

#include <type_traits>
#include <utility>

namespace quote{ namespace base{

	template <class... Procs>
	class procedure: public Procs...{
	protected:
		using tuple_type = ::std::tuple<Procs...>;

		procedure()
		{
		}

		procedure(Procs... procs)
			: Procs(procs)...
		{
		}

		template <template <class> class Pred>
		class make_filter{
		public:
			template <::std::size_t I>
			using type = Pred<typename ::std::tuple_element<I, tuple_type>::type>;
		};

		template <class Binder, bool Debug = false, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder, ::quote::tmp::tautological_filter, Debug>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <class> class Pred, bool Debug = false, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder, typename make_filter<Pred>::type, Debug>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <::std::size_t> class Filter, bool Debug = false, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder, Debug>(typename ::quote::tmp::make_filtered_index<Filter, sizeof...(Procs)>::type(), ::std::forward<Args>(args)...);
		}

		template <class Binder, bool Debug = false, ::std::size_t... Indices, class... Args>
		void for_each(::quote::tmp::index_tuple<Indices...> indices, Args... args)
		{
			auto to_void = [](...) {};
			to_void((Binder{}.operator()<Debug>(static_cast<::std::tuple_element_t<Indices, tuple_type>*>(this), static_cast<typename Binder::type*>(this), ::std::forward<Args>(args)...), 0)...);
		}

		template <class Binder, bool Debug = false, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder, ::quote::tmp::tautological_filter, Debug>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <class> class Pred, bool Debug = false, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder, typename make_filter<Pred>::type, Debug>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <::std::size_t> class Filter, bool Debug = false, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder, Debug>(typename ::quote::tmp::make_filtered_index<Filter, sizeof...(Procs)>::type(), std::forward<Args>(args)...);
		}

		template <class Binder, bool Debug = false, ::std::size_t Index, ::std::size_t... Rest, class... Args>
		bool all_of(::quote::tmp::index_tuple<Index, Rest...> indices, Args... args)
		{
			using result_type = decltype(Binder{}.operator()<Debug>(static_cast<::std::tuple_element_t<Index, tuple_type>*>(nullptr), static_cast<typename Binder::type*>(this), args...));
			return to_bool<result_type, Binder, Debug, Index>(args...)
				? all_of<Binder, Debug>(::quote::tmp::index_tuple<Rest...>(), ::std::forward<Args>(args)...)
				: false;
		}

		template <class Binder, bool Debug = false, class... Args>
		bool all_of(::quote::tmp::index_tuple<> indices, Args... args)
		{
			return true;
		}

	private:

		template <class Result, class Binder, bool Debug, ::std::size_t Index, class... Args>
		typename ::std::enable_if<::std::is_convertible<Result, bool>::value, bool>::type to_bool(Args... args)
		{
			return static_cast<bool>(Binder{}.operator()<Debug>(static_cast<::std::tuple_element_t<Index, tuple_type>*>(this), static_cast<typename Binder::type*>(this), args...));
		}
		template <class Result, class Binder, bool Debug, ::std::size_t Index, class... Args>
		typename std::enable_if<!::std::is_convertible<Result, bool>::value, bool>::type to_bool(Args... args)
		{
			Binder{}.operator()<Debug>(static_cast<::std::tuple_element_t<Index, tuple_type>*>(this), static_cast<typename Binder::type*>(this), args...);
			return true;
		}
	};

} }
