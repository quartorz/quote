#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>

#include <cassert>
#include <functional>
#include <tuple>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <random>

namespace quote{ namespace win32{

	template <class Derived>
	class resizer
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if(msg == WM_SIZE)
				static_cast<Derived*>(this)->on_size(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return true;
		}

		void set_size(int w, int h)
		{
			RECT rc;
			::GetWindowRect(static_cast<Derived*>(this)->get_hwnd(), &rc);
			POINT pt ={rc.left, rc.top};
			::ScreenToClient(::GetAncestor(static_cast<Derived*>(this)->get_hwnd(), GA_PARENT), &pt);
			::MoveWindow(static_cast<Derived*>(this)->get_hwnd(), pt.x, pt.y, w, h, TRUE);
		}
	};

	template <class Derived>
	class mover
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if(msg == WM_MOVE)
				static_cast<Derived*>(this)->on_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return true;
		}

		void set_position(int x, int y)
		{
			RECT rc;
			::GetWindowRect(static_cast<Derived*>(this)->get_hwnd(), &rc);
			::MoveWindow(static_cast<Derived*>(this)->get_hwnd(), x, y, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		}
	};

	template <class Derived>
	class on_close {
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if (msg == WM_CLOSE) {
				if (static_cast<Derived*>(this)->on_close()) {
					return true;
				}
				else {
					lresult = 0;
					return false;
				}
			}

			return true;
		}
	};

	template <class Derived>
	class quit_on_close
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if(msg == WM_DESTROY)
				::PostQuitMessage(0);
			return true;
		}
	};

	template <class Derived>
	class left_button
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_LBUTTONDOWN:
				static_cast<Derived*>(this)->on_left_press(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			case WM_LBUTTONUP:
				static_cast<Derived*>(this)->on_left_release(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			}

			return true;
		}
	};

	template <class Derived>
	class right_button
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_RBUTTONDOWN:
				static_cast<Derived*>(this)->on_right_press(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			case WM_RBUTTONUP:
				static_cast<Derived*>(this)->on_right_release(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			}

			return true;
		}
	};

	template <class Derived>
	class mouse_move
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if(msg == WM_MOUSEMOVE)
				static_cast<Derived*>(this)->on_mouse_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return true;
		}
	};

	template <class Derived>
	class keyboard
	{
		using handler_type = std::function<void(unsigned keycode, bool is_push)>;
		using hash_type = std::mt19937_64::result_type;
		using tuple_type = std::tuple<hash_type, handler_type>;
		using container_type = std::vector<tuple_type>;
		using map_type = std::array<container_type, 256>;
		map_type map;

		std::mt19937_64 rand{std::time(nullptr)};

	protected:
		using keycode_range = std::tuple<wchar_t, wchar_t>;

	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_KEYDOWN:
				for(auto &tuple : map[static_cast<unsigned>(wParam)]){
					std::get<1>(tuple)(static_cast<unsigned>(wParam), true);
				}
				static_cast<Derived*>(this)->on_key_down(static_cast<unsigned>(wParam));
				break;
			case WM_KEYUP:
				for(auto &tuple : map[static_cast<unsigned>(wParam)]){
					std::get<1>(tuple)(static_cast<unsigned>(wParam), false);
				}
				static_cast<Derived*>(this)->on_key_up(static_cast<unsigned>(wParam));
				break;
			}

			return true;
		}

		template <class Type, class... Types>
		hash_type add_keyboard_handler(handler_type handler, Type t, Types... characters)
		{
			hash_type hash = rand();
			auto tuple = tuple_type(hash, handler);

			add_handler_helper(tuple, t, characters...);

			return hash;
		}

		template <class... Types>
		void remove_keyboard_handler(hash_type hash, Types... characters)
		{
			remove_handler_helper(hash, characters...);
		}

	private:
		void add(tuple_type &tuple, wchar_t c)
		{
			assert(0 <= c && c <= 255);
			map[c].push_back(tuple);
		}

		void add_handler_helper(tuple_type &)
		{
		}

		template <class... Others>
		void add_handler_helper(tuple_type &tuple, wchar_t c, Others... others)
		{
			add(tuple, c);
			add_handler_helper(tuple, others...);
		}

		template <class... Others>
		void add_handler_helper(tuple_type &tuple, keycode_range &range, Others... others)
		{
			wchar_t start = std::get<0>(range);
			wchar_t end = std::get<1>(range);
			if(start <= end){
				do{
					add(tuple, start);
				} while(++start <= end);
			}else{
				do{
					add(tuple, start);
				} while(--start >= end);
			}
			add_handler_helper(tuple, others...);
		}

		void remove(hash_type hash, wchar_t c)
		{
			auto it = map.find(c);
			if(it == map.end())
				return;

			auto &container = it->second;
			auto iter = std::find_if(container.begin(), container.end(), [&hash](container_type::value_type &v){return std::get<0>(v) == hash;});
			if(iter != container.end())
				container.erase(iter);
		}

		void remove_handler_helper(hash_type)
		{
		}

		template <class... Others>
		void remove_handler_helper(hash_type hash, wchar_t c, Others... others)
		{
			remove(hash, c);
			remove_handler_helper(hash, others...);
		}

		template <class... Others>
		void remove_handler_helper(hash_type hash, std::tuple<wchar_t, wchar_t> &range, Others... others)
		{
			wchar_t start = std::get<0>(range);
			wchar_t end = std::get<1>(range);
			if(start <= end){
				do{
					remove(hash, start);
				} while(++start <= end);
			}else{
				do{
					remove(hash, start);
				} while(--start >= end);
			}
			remove_handler_helper(hash, others...);
		}
	};

	template <class Derived>
	class timer
	{
		using handler_type = std::function<void(unsigned id)>;
		using hash_type = std::mt19937_64::result_type;
		using tuple_type = std::tuple<hash_type, handler_type>;
		using id_type = unsigned;
		using container_type = std::vector<tuple_type>;
		using map_type = std::unordered_map<id_type, container_type>;
		map_type map;

		std::mt19937_64 rand{std::time(nullptr)};

	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			if(msg == WM_TIMER){
				auto it = map.find(static_cast<id_type>(wParam));
				if(it != map.end()){
					auto &container = it->second;
					for(auto &tuple: container){
						std::get<1>(tuple)(static_cast<id_type>(wParam));
					}
				}
				static_cast<Derived*>(this)->on_timer(static_cast<id_type>(wParam));
			}

			return true;
		}

		bool set_timer(unsigned id, UINT time)
		{
			return ::SetTimer(static_cast<Derived*>(this)->GetHwnd(), id, time, NULL) != 0;
		}

		void kill_timer(unsigned id)
		{
			::KillTimer(static_cast<Derived*>(this)->GetHwnd(), id);
		}

		template <class Integer, class... Integers>
		hash_type add_timer_handler(handler_type handler, Integer id, Integers... ids)
		{
			hash_type hash = rand();
			add_handler_helper(tuple_type(hash, handler), id, ids...);

			return hash;
		}

		template <class... Integers>
		void remove_timer_handler(hash_type hash, Integers... ids)
		{
			remove_handler_helper(hash, ids...);
		}

	private:
		void add_handler_helper(tuple_type &)
		{
		}

		template <class... Integers>
		void add_handler_helper(tuple_type &tuple, unsigned id, Integers... ids)
		{
			map[id].push_back(tuple);
			add_handler_helper(tuple, ids...);
		}

		void remove_handler_helper(hash_type)
		{
		}

		template <class... Integers>
		void remove_handler_helper(hash_type hash, unsigned id, Integers... ids)
		{
			auto it = map.find(id);
			if(it == map.end())
				return;

			auto &container = it->second;
			auto iter = std::find_if(container.begin(), container.end(), [&hash](container_type::value_type &v){return std::get<0>(v) == hash;});
			if(iter != container.end())
				container.erase(iter);

			remove_handler_helper(hash, ids...);
		}
	};

	template <class Derived>
	class drop_files
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_CREATE:
				::DragAcceptFiles(hwnd, TRUE);
				break;

			case WM_DROPFILES:
				{
					 auto hdrop = reinterpret_cast<HDROP>(wParam);

					 auto c = ::DragQueryFileW(hdrop, -1, nullptr, 0);

					 std::size_t size = 0;
					 std::vector<wchar_t> file;
					 std::vector<std::wstring> files;
					 for(decltype(c) i = 0; i < c; ++i){
						 size = ::DragQueryFileW(hdrop, i, nullptr, 0);
						 file.resize(size + 1);
						 ::DragQueryFileW(hdrop, i, &file[0], size + 1);
						 files.emplace_back(file.begin(), file.end());
					 }

					 static_cast<Derived*>(this)->on_drop_files(files);
				}
				break;
			}

			return true;
		}
	};

	template <class Derived>
	class focus
	{
	public:
		bool WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_SETFOCUS:
				static_cast<Derived*>(this)->on_get_focus((HWND)wParam);
				break;
			case WM_KILLFOCUS:
				static_cast<Derived*>(this)->on_lost_focus((HWND)wParam);
			}

			return true;
		}
	};

} }
