#pragma once

#include <Windows.h>

namespace quote{ namespace win32{

	template <class Derived, class Traits>
	class object_processor{
		HCURSOR hCursor[3];
		typename Traits::object *focus, *hover;
		bool inside, pushing;

	protected:
		enum cursor_type{
			arrow,
			hand,
			ibeam
		}cursortype;
		void set_cursor_type(cursor_type);

	public:
		object_processor();
		~object_processor();
		bool WindowProc(HWND, UINT, WPARAM, LPARAM, LRESULT &);
	};

} }

#include "details/object_processor.hpp"
