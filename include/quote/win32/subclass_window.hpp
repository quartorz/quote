#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include "../tmp/nil.hpp"

#include <tuple>

#include <string>

#pragma comment(lib, "Comctl32")

namespace quote{ namespace win32{

	template <class Derived, class... Procs>
	class subclass_window: public Procs...{
		static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uId, DWORD_PTR dwRefData)
		{
			auto w = reinterpret_cast<Derived*>(dwRefData);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

	protected:
		HWND hwnd, hparent;

	public:
		static const bool is_subclass = true;
		bool set_subclass(HWND hwnd)
		{
			::SetWindowSubclass(hwnd, SubclassProc, 0, reinterpret_cast<DWORD_PTR>(this));
			this->hwnd = hwnd;
			hparent = ::GetAncestor(hwnd, GA_PARENT);
			return static_cast<Derived*>(this)->initialize();
		}

		subclass_window(): hwnd(nullptr), hparent(nullptr)
		{
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

		void enable(void) const
		{
			::EnableWindow(hwnd, TRUE);
		}

		void disable(void) const
		{
			::EnableWindow(hwnd, FALSE);
		}

		void show(void)
		{
			::ShowWindow(hwnd, SW_SHOW);
		}

		void hide(void)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}

		void destroy()
		{
			::DestroyWindow(hwnd);
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

			return ::DefSubclassProc(hwnd, msg, wParam, lParam);
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
