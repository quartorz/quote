#pragma once

#include "base_types.hpp"
#include "resource.hpp"

namespace quote{ namespace direct2d{

	class object: public resource{
	protected:
		point pos;
		size size_;

	public:
		struct hittest{
			HWND hwnd;

			enum class Cursor: int{
				Arrow,
				Hand,
				Ibeam,
			}cursor;

			void set_cursor(Cursor c)
			{
				cursor = c;
			}
		};
		template <class Window>
		static hittest create_hittest(Window *w)
		{
			return {w->get_hwnd(), hittest::Cursor::Arrow};
		}
	};

} }
