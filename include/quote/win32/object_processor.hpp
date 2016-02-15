#pragma once

#include <Windows.h>

namespace quote{ namespace win32{

	template <class Derived, class Traits>
	class object_processor{
		HCURSOR hCursor[4];
		typename Traits::object *focus, *hover;
		bool inside, pushing;

	protected:
		enum cursor_type{
			arrow,
			hand,
			ibeam,
			leftright
		}cursortype;
		void set_cursor_type(cursor_type);

	public:
		object_processor();
		~object_processor();
		bool WindowProc(Derived &, HWND, UINT, WPARAM, LPARAM, LRESULT &);
	};

} }

#include <quote/win32/impl/object_processor.hpp>
