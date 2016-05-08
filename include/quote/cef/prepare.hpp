#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"

namespace quote{ namespace cef{

	bool prepare(int &exit_code, void *&sandbox_info)
	{
		::CefEnableHighDPISupport();

		CefMainArgs main_args(::GetModuleHandleW(nullptr));
		exit_code = ::CefExecuteProcess(main_args, nullptr, sandbox_info);
		return exit_code >= 0;
	}

	bool prepare(CefApp *app, int &exit_code, void *&sandbox_info)
	{
		::CefEnableHighDPISupport();

		CefMainArgs main_args(::GetModuleHandleW(nullptr));
		exit_code = ::CefExecuteProcess(main_args, app, sandbox_info);
		return exit_code >= 0;
	}

	bool prepare(CefApp *app, const CefMainArgs &main_args, int &exit_code, void *&sandbox_info)
	{
		::CefEnableHighDPISupport();
		exit_code = ::CefExecuteProcess(main_args, app, sandbox_info);
		return exit_code >= 0;
	}

} }
