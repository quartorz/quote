#pragma once

#include "../scene.hpp"
#include "../tmp/applier.hpp"

namespace quote{

	template <class Traits, class... Options>
	void scene<Traits, Options...>::on_show()
	{
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::on_hide()
	{
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::on_key_down(unsigned keycode)
	{
		assert(0 <= keycode && keycode <= 255);

		::quote::tmp::make_applier(option_keyboard)(on_key_down_binder(), *this, keycode);

		for(auto &tuple : kb_map[keycode]){
			std::get<1>(tuple)(keycode, true);
		}
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::on_key_up(unsigned keycode)
	{
		assert(0 <= keycode && keycode <= 255);

		::quote::tmp::make_applier(option_keyboard)(on_key_up_binder(), *this, keycode);

		for(auto &tuple: kb_map[keycode]){
			std::get<1>(tuple)(keycode, false);
		}
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::on_timer(unsigned id)
	{
		auto it = timer_map.find(id);
		if(it != timer_map.end()){
			for(auto &handler: it->second){
				std::get<1>(handler)(id);
			}
		}
	}

	template <class Traits, class... Options>
	template <class Type, class... Types>
	typename scene<Traits, Options...>::hash_type scene<Traits, Options...>::add_keyboard_handler(kb_handler_type handler, Type t, Types... characters)
	{
		hash_type hash = rand();
		auto tuple = kb_tuple_type(hash, handler);

		add_keyboard_handler_helper(tuple, t, characters...);

		return hash;
	}

	template <class Traits, class... Options>
	template <class... Types>
	void scene<Traits, Options...>::remove_keyboard_handler(hash_type hash, Types... characters)
	{
		remove_keyboard_handler_helper(hash, characters...);
	}

	template <class Traits, class... Options>
	template <class Integer, class... Integers>
	typename scene<Traits, Options...>::hash_type scene<Traits, Options...>::add_timer_handler(timer_handler_type handler, Integer id, Integers... ids)
	{
		hash_type hash = rand();
		add_timer_handler_helper(timer_tuple_type(hash, handler), id, ids...);

		return hash;
	}

	template <class Traits, class... Options>
	template <class... Integers>
	void scene<Traits, Options...>::remove_timer_handler(hash_type hash, Integers... ids)
	{
		remove_timer_handler_helper(hash, ids...);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::add_keyboard_handler_helper2(kb_tuple_type &tuple, wchar_t c)
	{
		assert(0 <= c && c <= 255);
		kb_map[c].push_back(tuple);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::add_keyboard_handler_helper(kb_tuple_type &)
	{
	}

	template <class Traits, class... Options>
	template <class... Others>
	void scene<Traits, Options...>::add_keyboard_handler_helper(kb_tuple_type &tuple, wchar_t c, Others... others)
	{scene<Traits, Options...>::
		add_keyboard_handler_helper2(tuple, c);
		add_keyboard_handler_helper(tuple, others...);
	}

	template <class Traits, class... Options>
	template <class... Others>
	void scene<Traits, Options...>::add_keyboard_handler_helper(kb_tuple_type &tuple, keycode_range &range, Others... others)
	{
		wchar_t start = std::get<0>(range);
		wchar_t end = std::get<1>(range);
		if(start <= end){
			do{
				add_keyboard_handler_helper2(tuple, start);
			}while(++start <= end);
		} else{
			do{
				add_keyboard_handler_helper2(tuple, start);
			}while(--start >= end);
		}
		add_keyboard_handler_helper(tuple, others...);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::remove_keyboard_handler_helper2(hash_type hash, wchar_t c)
	{
		auto it = kb_map.find(c);
		if(it == kb_map.end())
			return;

		auto &container = it->second;
		auto iter = std::find_if(container.begin(), container.end(), [&hash](kb_container_type::value_type &v){return std::get<0>(v) == hash;});
		if(iter != container.end())
			container.erase(iter);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::remove_keyboard_handler_helper(hash_type)
	{
	}

	template <class Traits, class... Options>
	template <class... Others>
	void scene<Traits, Options...>::remove_keyboard_handler_helper(hash_type hash, wchar_t c, Others... others)
	{
		remove_keyboard_handler_helper2(hash, c);
		remove_keyboard_handler_helper(hash, others...);
	}

	template <class Traits, class... Options>
	template <class... Others>
	void scene<Traits, Options...>::remove_keyboard_handler_helper(hash_type hash, keycode_range &range, Others... others)
	{
		wchar_t start = std::get<0>(range);
		wchar_t end = std::get<1>(range);
		if(start <= end){
			do{
				remove_keyboard_handler_helper2(hash, start);
			}while(++start <= end);
		} else{
			do{
				remove_keyboard_handler_helper2(hash, start);
			}while(--start >= end);
		}
		remove_keyboard_handler_helper(hash, others...);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::add_timer_handler_helper(timer_tuple_type &)
	{
	}

	template <class Traits, class... Options>
	template <class... Integers>
	void scene<Traits, Options...>::add_timer_handler_helper(timer_tuple_type &tuple, unsigned id, Integers... ids)
	{
		timer_map[id].push_back(tuple);
		add_timer_handler_helper(tuple, ids...);
	}

	template <class Traits, class... Options>
	void scene<Traits, Options...>::remove_timer_handler_helper(hash_type)
	{
	}

	template <class Traits, class... Options>
	template <class... Integers>
	void scene<Traits, Options...>::remove_timer_handler_helper(hash_type hash, unsigned id, Integers... ids)
	{
		auto it = timer_map.find(id);
		if(it == timer_map.end())
			return;

		auto &container = it->second;
		auto iter = std::find_if(container.begin(), container.end(), [&hash](timer_container_type::value_type &v){return std::get<0>(v) == hash;});
		if(iter != container.end())
			container.erase(iter);
	}

}
