#pragma once

#include <cef_base.h>
#include <cef_app.h>

namespace quote{ namespace cef{

	struct message_handler {
		void on_no_message()
		{
			::CefDoMessageLoopWork();
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
