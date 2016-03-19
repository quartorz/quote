#pragma once

#include <Windows.h>

#include <quote/base/procedure.hpp>
#include <quote/macro.hpp>

#include <chrono>
#include <thread>

namespace quote{ namespace win32{

	namespace detail {

		template <typename... Procs>
		class message_loop_procedure : public ::quote::base::procedure<Procs...> {
			using base = ::quote::base::procedure<Procs...>;

			QUOTE_DEFINE_BINDER(message_loop_procedure<Procs...>, on_key_down);
			QUOTE_DEFINE_BINDER(message_loop_procedure<Procs...>, on_no_message);

		public:
			message_loop_procedure(Procs... p)
				: base(p...)
			{
			}
			bool on_key_down(unsigned keycode)
			{
				return this->all_of<on_key_down_binder>(keycode);
			}
			void on_no_message()
			{
				this->for_each<on_no_message_binder>();
			}
		};

		template <typename F, bool>
		class on_key_down_wrapper {
			F f_;

		public:
			on_key_down_wrapper(F f)
				: f_(f)
			{
			}

			template <typename Proc>
			bool on_key_down(Proc &, unsigned keycode)
			{
				return static_cast<bool>(f_(keycode));
			}
		};

		template <typename F>
		class on_key_down_wrapper<F, false> {
			F f_;

		public:
			on_key_down_wrapper(F f)
				: f_(f)
			{
			}

			template <typename Proc>
			bool on_key_down(Proc &, unsigned keycode)
			{
				f_(keycode);
				return true;
			}
		};

		template <typename F>
		class on_no_message_wrapper {
			F f_;

		public:
			on_no_message_wrapper(F f)
				: f_(f)
			{
			}

			template <typename Proc>
			void on_no_message(Proc &)
			{
				f_();
			}
		};

	}

	template <typename F>
	inline detail::on_key_down_wrapper<
		F,
		::std::is_convertible<::std::result_of_t<F(unsigned)>, bool>::value
	>
	key_down(F f)
	{
		return{ f };
	}

	template <typename F>
	inline detail::on_no_message_wrapper<F> no_message(F f)
	{
		return{ f };
	}

	template <typename... Procs>
	inline int message_loop(Procs... procs)
	{
		auto procedure = detail::message_loop_procedure<Procs...>(procs...);

		BOOL ret;
		MSG msg;
		do {
			if (::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
				ret = ::GetMessageW(&msg, nullptr, 0, 0);
				if (ret == 0 || ret == -1)
					break;
				if (msg.message == WM_KEYDOWN
					&& !procedure.on_key_down(static_cast<unsigned>(msg.wParam)))
					continue;
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			} else {
				::Sleep(1);
				procedure.on_no_message();
			}
		} while (msg.message != WM_QUIT);

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

		template <typename Proc>
		void on_no_message(Proc &)
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
