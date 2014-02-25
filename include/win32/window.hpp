#pragma once

#include <Windows.h>
#include <windowsx.h>

#include "../tmp/nil.hpp"

#include <tuple>

namespace quote{ namespace win32{

	template <class Derived, class... Procs>
	class window: public Procs...{
		static LRESULT CALLBACK WindowProc_SetData(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(msg == WM_NCCREATE || msg == WM_CREATE){
				auto lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(lpcs->lpCreateParams));
				::SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc_Static));
				w = reinterpret_cast<Derived*>(lpcs->lpCreateParams);
				w->hwnd = hwnd;
				w->hparent = lpcs->hwndParent;
			}
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

		static LRESULT CALLBACK WindowProc_Static(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

	protected:
		HWND hwnd, hparent;

	public:
		window(): hwnd(nullptr), hparent(nullptr)
		{
		}

		static bool register_class(const wchar_t *classname)
		{
			WNDCLASSEXW wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WindowProc_SetData;
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

		void show(void)
		{
			::ShowWindow(hwnd, SW_SHOW);
		}

		void hide(void)
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

		LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg){
			case WM_CREATE:
				if(!static_cast<Derived*>(this)->initialize())
					return -1l;
				break;
			case WM_DESTROY:
				static_cast<Derived*>(this)->uninitialize();
				break;
			}

			if(sizeof...(Procs) > 0){
				LRESULT lresult = 0l;
				if(!call_proc<Procs..., quote::tmp::nil>(hwnd, msg, wParam, lParam, lresult))
					return lresult;
			}

			if(msg == WM_DESTROY)
				this->hwnd = nullptr;

			return ::DefWindowProcW(hwnd, msg, wParam, lParam);
		}

	private:
		// FIXME
		template <class Proc, class... Others>
		bool call_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			return this->Proc::WindowProc(hwnd, msg, wParam, lParam, lresult)
				&& call_proc<Others...>(hwnd, msg, wParam, lParam, lresult);
		}

		template <>
		bool call_proc<quote::tmp::nil>(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			return true;
		}
	};

} }
