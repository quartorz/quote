#pragma once

#include <cef_base.h>
#include <cef_app.h>

namespace quote{ namespace cef{

	bool prepare(int &exit_code, void *&sandbox_info)
	{
		::CefEnableHighDPISupport();

		CefMainArgs main_args(::GetModuleHandleW(nullptr));
		exit_code = ::CefExecuteProcess(main_args, nullptr, sandbox_info);
		return exit_code >= 0;
	}

} }
