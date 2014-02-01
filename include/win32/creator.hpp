#pragma once

#include <Windows.h>

#include <climits>

namespace quote{ namespace win32{

	struct creation_params{
		DWORD exstyle, style;
		const wchar_t *classname, *title;
		int x, y, w, h;
		HWND hparent;
	};

	template <class Derived>
	class creator{
	public:
		bool create(creation_params &param)
		{
			if(param.w != CW_USEDEFAULT){
				RECT rc = {0, 0, param.w, param.h};
				::AdjustWindowRectEx(&rc, param.style, FALSE, param.exstyle);
				param.w = rc.right - rc.left;
				param.h = rc.bottom - rc.top;
			}
			HWND hwnd = ::CreateWindowExW(
				param.exstyle,
				param.classname,
				param.title,
				param.style,
				param.x,
				param.y,
				param.w,
				param.h,
				param.hparent,
				nullptr,
				::GetModuleHandleW(nullptr),
				static_cast<Derived*>(this));
			return hwnd != nullptr;
		}

		template <class Parent>
		bool create(Parent &parent, const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
		{
			creation_params params ={
				0,
				WS_OVERLAPPEDWINDOW,
				((classname != nullptr) ? classname : static_cast<Derived*>(this)->get_class_name())
				title,
				((x == INT_MAX || y == INT_MAX) ? CW_USEDEFAULT : x),
				y,
				((w == -1 || h == -1) ? CW_USEDEFAULT : w),
				h,
				parent.get_hwnd(),
			};

			return create(params);
		}

		bool create(const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
		{
			creation_params params ={
				0,
				WS_OVERLAPPEDWINDOW,
				((classname != nullptr) ? classname : static_cast<Derived*>(this)->get_class_name()),
				title,
				((x == INT_MAX || y == INT_MAX) ? CW_USEDEFAULT : x),
				y,
				((w == -1 || h == -1) ? CW_USEDEFAULT : w),
				h,
				nullptr,
			};

			return create(params);
		}
	};

} }
