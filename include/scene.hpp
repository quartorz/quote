#pragma once

#include <functional>
#include <tuple>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>
#include <cassert>

namespace quote{

	template <class Traits, bool MultiThread = false>
	class scene: public Traits::template userdefined_object<scene<Traits, MultiThread>, false, MultiThread>
	{
		using hash_type = std::mt19937_64::result_type;

		using kb_handler_type = std::function<void(unsigned keycode, bool is_push)>;
		using kb_tuple_type = std::tuple<hash_type, kb_handler_type>;
		using kb_container_type = std::vector<kb_tuple_type>;
		using kb_map_type = std::array<kb_container_type, 256>;
		kb_map_type kb_map;

		using timer_handler_type = std::function<void(unsigned id)>;
		using timer_tuple_type = std::tuple<hash_type, timer_handler_type>;
		using timer_id_type = unsigned;
		using timer_container_type = std::vector<timer_tuple_type>;
		using timer_map_type = std::unordered_map<timer_id_type, timer_container_type>;
		timer_map_type timer_map;

		std::mt19937_64 rand{std::time(nullptr)};

	protected:
		using keycode_range = std::tuple<wchar_t, wchar_t>;

	public:
		virtual void on_show();
		virtual void on_hide();
		virtual void on_key_down(unsigned keycode);
		virtual void on_key_up(unsigned keycode);
		virtual void on_timer(unsigned id);
		
		template <class Type, class... Types>
		hash_type add_keyboard_handler(kb_handler_type handler, Type t, Types... characters);

		template <class... Types>
		void remove_keyboard_handler(hash_type hash, Types... characters);

		template <class Integer, class... Integers>
		hash_type add_timer_handler(timer_handler_type handler, Integer id, Integers... ids);

		template <class... Integers>
		void remove_timer_handler(hash_type hash, Integers... ids);

	private:
		void add_keyboard_handler_helper2(kb_tuple_type &tuple, wchar_t c);
		void add_keyboard_handler_helper(kb_tuple_type &);
		template <class... Others>
		void add_keyboard_handler_helper(kb_tuple_type &tuple, wchar_t c, Others... others);
		template <class... Others>
		void add_keyboard_handler_helper(kb_tuple_type &tuple, keycode_range &range, Others... others);

		void remove_keyboard_handler_helper2(hash_type hash, wchar_t c);
		void remove_keyboard_handler_helper(hash_type);
		template <class... Others>
		void remove_keyboard_handler_helper(hash_type hash, wchar_t c, Others... others);
		template <class... Others>
		void remove_keyboard_handler_helper(hash_type hash, keycode_range &range, Others... others);

		void add_timer_handler_helper(timer_tuple_type &);
		template <class... Integers>
		void add_timer_handler_helper(timer_tuple_type &tuple, unsigned id, Integers... ids);

		void remove_timer_handler_helper(hash_type);
		template <class... Integers>
		void remove_timer_handler_helper(hash_type hash, unsigned id, Integers... ids);
	};

}

#include "details/scene.hpp"
