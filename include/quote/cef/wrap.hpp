#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"

#include <chrono>

namespace quote{ namespace cef{

	class message_handler {
		std::chrono::system_clock::time_point last_time_;

	public:
		message_handler()
			: last_time_(std::chrono::system_clock::now())
		{
		}

		template <typename Proc>
		void on_no_message(Proc &)
		{
			using namespace std::chrono;

			auto now = system_clock::now();

			if (duration_cast<milliseconds>(now - last_time_).count() >= 33) {
				::CefDoMessageLoopWork();

				last_time_ = now;
			}
		}
	};

	template <typename Func>
	int wrap(CefApp *app, void *sandbox_info, Func f)
	{
		CefMainArgs main_args(::GetModuleHandleW(nullptr));
		CefSettings settings;

		settings.multi_threaded_message_loop = false;
		settings.single_process = false;

		::CefInitialize(main_args, settings, app, sandbox_info);

		int code = f();

		::CefDoMessageLoopWork();

		::CefShutdown();

		return code;
	}

	template <typename Func>
	int wrap(CefApp *app, const CefMainArgs &main_args, void *sandbox_info, Func f)
	{
		CefSettings settings;

		settings.multi_threaded_message_loop = false;
		settings.single_process = false;

		::CefInitialize(main_args, settings, app, sandbox_info);

		int code = f();

		::CefDoMessageLoopWork();

		::CefShutdown();

		return code;
	}

} }
