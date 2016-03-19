#pragma once

#include <cef_base.h>
#include <cef_app.h>

#include <chrono>

namespace quote{ namespace cef{

	class message_handler {
		std::chrono::system_clock::time_point last_;

	public:
		message_handler()
			: last_(std::chrono::system_clock::now())
		{
		}

		template <typename Proc>
		void on_no_message(Proc &)
		{
			using namespace std::chrono;

			auto now = system_clock::now();

			if (duration_cast<milliseconds>(now - last_).count() >= 1000) {
				::CefDoMessageLoopWork();

				last_ = now;
				::OutputDebugStringW(L"a\n");
			}
		}
	};

	template <typename App, typename Func>
	int wrap(App *app, void *sandbox_info, Func f)
	{
		CefMainArgs main_args(::GetModuleHandleW(nullptr));
		CefSettings settings;

		::CefInitialize(main_args, settings, app, sandbox_info);

		int code = f();

		::CefShutdown();

		return code;
	}

} }
