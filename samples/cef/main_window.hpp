#pragma once

#include <quote/win32/window.hpp>
#include <quote/win32/creator.hpp>
#include <quote/win32/procs.hpp>

#include <quote/direct2d/painter.hpp>

#include <quote/cef/app.hpp>

class main_window :
	public quote::win32::window<
		main_window,
		quote::win32::quit_on_close<main_window>,
		quote::direct2d::painter<main_window>
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

	void draw(quote::direct2d::paint_params &pp)
	{
	}

	bool create_resource(quote::direct2d::creation_params &cp)
	{
		return true;
	}

	void destroy_resource()
	{
	}
};
