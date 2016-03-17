#pragma once

#include <quote/win32/window.hpp>
#include <quote/win32/creator.hpp>
#include <quote/win32/procs.hpp>

#include <quote/cef/app.hpp>

class main_window :
	public quote::win32::window<
		main_window,
		quote::win32::quit_on_close<main_window>
	>,
	public quote::win32::creator<main_window>,
	public quote::cef::app<main_window>
{
public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}

	bool initialize()
	{
		auto hinstance = ::GetModuleHandleW(nullptr);

		CefMainArgs main_args(hinstance);

		CefSettings settings;

		return true;
	}

	void uninitialize()
	{
	}
};
