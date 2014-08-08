#pragma once

#include <Windows.h>

#include <climits>

#include "../tmp/has_xxx.hpp"

namespace quote{ namespace win32{

	namespace aux{
		QUOTE_DECL_HAS_NON_TYPE(is_subclass)
	}

	struct creation_params{
		DWORD exstyle, style;
		const wchar_t *classname, *title;
		int x, y, w, h;
		HWND hparent;
	};

	template <class Derived>
	class creator{
		template <bool>
		void set_subclass_impl(HWND hwnd)
		{
			static_cast<Derived*>(this)->set_subclass(hwnd);
		}
		template <>
		void set_subclass_impl<false>(HWND)
		{
		}

		void set_subclass_(std::true_type&, HWND hwnd)
		{
			set_subclass_impl<Derived::is_subclass>(hwnd);
		}
		void set_subclass_(std::false_type&, HWND)
		{
		}

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
			set_subclass_(typename aux::has_is_subclass<Derived>::type(), hwnd);
			return hwnd != nullptr;
		}

		template <class Parent>
		bool create(Parent &parent, const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
		{
			creation_params params ={
				0,
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
				((classname != nullptr) ? classname : static_cast<Derived*>(this)->get_class_name()),
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
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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
