#pragma once

#include <Windows.h>

#include "../tmp/combiner.hpp"

#include <chrono>
#include <thread>

namespace quote{ namespace win32{

	template <class Func>
	class key_hook_class{
		Func fn;
	public:
		key_hook_class(Func f): fn(f)
		{
		}
		bool operator()(unsigned key)
		{
			return fn(key);
		}
	};

	template <class Func>
	key_hook_class<Func> key_hook(Func f)
	{
		return {f};
	}

	template <class Func>
	inline int message_loop(key_hook_class<Func> f)
	{
		BOOL ret;
		MSG msg;
		while((ret = ::GetMessageW(&msg, nullptr, 0, 0)) == 1){
			if(msg.message == WM_KEYDOWN && !f(msg.wParam)){
			}else{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}

		return static_cast<int>(msg.wParam);
	}

	template <class Func, class... Functions>
	inline int message_loop(key_hook_class<Func> f, Functions... functions)
	{
		auto combiner = quote::tmp::make_combiner(functions...);

		BOOL ret;
		MSG msg;
		do{
			if(::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)){
				ret = ::GetMessageW(&msg, nullptr, 0, 0);
				if(ret == 0 || ret == -1)
					break;
				if(msg.message == WM_KEYDOWN && !f(msg.wParam)){
				}else{
					::TranslateMessage(&msg);
					::DispatchMessageW(&msg);
				}
			}else{
				combiner();
			}
		}while(msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}

	template <class... Functions>
	// アイドル時にfunctionsを呼び出す
	inline int message_loop(Functions... functions)
	{
		auto combiner = quote::tmp::make_combiner(functions...);

		BOOL ret;
		MSG msg;
		do{
			if(::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)){
				ret = ::GetMessageW(&msg, nullptr, 0, 0);
				if(ret == 0 || ret == -1)
					break;
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}else{
				combiner();
			}
		}while(msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}

	inline int message_loop(void)
	{
		BOOL ret;
		MSG msg;
		while((ret = ::GetMessageW(&msg, nullptr, 0, 0)) == 1){
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}

		return static_cast<int>(msg.wParam);
	}

	template <class Window, unsigned ID = UINT_MAX /* std::numeric_limits<unsigned>::max() */, unsigned FPS = 60>
	class frame_controller{
		Window &window;
		std::chrono::system_clock::time_point first;
		int count = 0;
		bool flag = true;

	public:
		frame_controller(Window &w): window(w)
		{
		}

		void operator()()
		{
			using namespace std::chrono;
			using std::this_thread::sleep_for;

			milliseconds duration;

			if(count == 0){
				if(flag){
					flag = false;
				}else{
					duration = duration_cast<milliseconds>(first - system_clock::now()) + seconds(1);
				}
			}else{
				duration = duration_cast<milliseconds>(first - system_clock::now()) + count * milliseconds(1000) / FPS;
			}

			sleep_for(duration);

			auto now = system_clock::now();

			if(count == 0)
				first = now;

			++count;
			if(count >= FPS)
				count = 0;

			window.WindowProc(window.get_hwnd(), WM_TIMER, ID, 0);
			window.repaint();
		}
	};

	template <unsigned ID = UINT_MAX, unsigned FPS = 60, class Window>
	frame_controller<Window, ID, FPS> make_frame_controller(Window &window)
	{
		return frame_controller<Window, ID, FPS>(window);
	}

} }
