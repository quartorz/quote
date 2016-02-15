#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <quote/base/procedure.hpp>
#include <quote/macro.hpp>

#include <tuple>

#include <string>

namespace quote{ namespace win32{

	template <class Derived, class... Procs>
	class window: public ::quote::base::procedure<Procs...>{
		QUOTE_DECLARE_BINDER(Derived, repaint)
		QUOTE_DECLARE_BINDER(Derived, WindowProc, windowproc_binder)
		QUOTE_DECLARE_BINDER(Derived, initialize)
		QUOTE_DECLARE_BINDER(Derived, uninitialize)

		static LRESULT CALLBACK WindowProc0(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(msg == WM_NCCREATE || msg == WM_CREATE){
				auto lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(lpcs->lpCreateParams));
				::SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc1));
				w = reinterpret_cast<Derived*>(lpcs->lpCreateParams);
				w->hwnd = hwnd;
				w->hparent = lpcs->hwndParent;
			}
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

		static LRESULT CALLBACK WindowProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

	protected:
		HWND hwnd, hparent;

		using window_type = window<Derived, Procs...>;

	public:
		window(): hwnd(nullptr), hparent(nullptr)
		{
		}

		static bool register_class(const wchar_t *classname)
		{
			WNDCLASSEXW wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WindowProc0;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(void*);
			wcex.hInstance = ::GetModuleHandleW(NULL);
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = nullptr;
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = classname;
			wcex.hIconSm = nullptr;

			return ::RegisterClassExW(&wcex) != 0;
		}

		static bool register_class(void)
		{
			return register_class(Derived::get_class_name());
		}

		void *get_handle() const
		{
			return hwnd;
		}

		HWND get_hwnd() const
		{
			return hwnd;
		}

		HWND get_parent() const
		{
			return hparent;
		}

		void enable() const
		{
			::EnableWindow(hwnd, TRUE);
		}

		void disable() const
		{
			::EnableWindow(hwnd, FALSE);
		}

		void show()
		{
			::ShowWindow(hwnd, SW_SHOW);
		}

		void hide()
		{
			::ShowWindow(hwnd, SW_HIDE);
		}

		std::tuple<int, int> get_size()
		{
			RECT rc;
			::GetClientRect(hwnd, &rc);
			return std::tuple<int, int>(rc.right, rc.bottom);
		}

		std::tuple<int, int> get_position()
		{
			RECT rc;
			::GetWindowRect(hwnd, &rc);
			return std::tuple<int, int>(rc.left, rc.top);
		}

		void set_title(const wchar_t *title)
		{
			::SetWindowTextW(hwnd, title);
		}

		void set_title(const std::wstring &title)
		{
			::SetWindowTextW(hwnd, title.c_str());
		}

		std::wstring get_title()
		{
			int n = ::GetWindowTextLengthW(hwnd);
			wchar_t *buf = reinterpret_cast<wchar_t*>(_malloca(sizeof(wchar_t) * (n + 1)));

			::GetWindowTextW(hwnd, buf, n + 1);

			std::wstring title(buf);

			::_freea(buf);

			return title;
		}

		LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg){
			case WM_CREATE:
				if(!static_cast<Derived*>(this)->initialize() || !this->all_of<initialize_binder>())
					return -1l;
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			case WM_DESTROY:
				static_cast<Derived*>(this)->uninitialize();
				this->for_each<uninitialize_binder>();
				break;
			}

			if(sizeof...(Procs) > 0){
				LRESULT lresult = 0l;
				if(!this->all_of<windowproc_binder>(hwnd, msg, wParam, lParam, ::std::ref(lresult)))
					return lresult;
			}

			if(msg == WM_DESTROY)
				this->hwnd = nullptr;

			return ::DefWindowProcW(hwnd, msg, wParam, lParam);
		}
	};

} }
