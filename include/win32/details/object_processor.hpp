#pragma once

#include "../object_processor.hpp"

#include <windowsx.h>

namespace quote{ namespace win32{

	template <class Derived, class Traits>
	void object_processor<Derived, Traits>::set_cursor_type(cursor_type t)
	{
		cursortype = t;
		::SetCursor(hCursor[t]);
	}

	template <class Derived, class Traits>
	object_processor<Derived, Traits>::object_processor():
		cursortype(arrow), focus(nullptr), hover(nullptr), inside(false), pushing(false)
	{
		hCursor[0] = static_cast<HCURSOR>(::LoadImageW(
			nullptr,
			IDC_ARROW,
			IMAGE_CURSOR,
			0,
			0,
			LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_SHARED));
		hCursor[1] = static_cast<HCURSOR>(::LoadImageW(
			nullptr,
			IDC_HAND,
			IMAGE_CURSOR,
			0,
			0,
			LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_SHARED));
		hCursor[2] = static_cast<HCURSOR>(::LoadImageW(
			nullptr,
			IDC_IBEAM,
			IMAGE_CURSOR,
			0,
			0,
			LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_SHARED));
		hCursor[3] = static_cast<HCURSOR>(::LoadImageW(
			nullptr,
			IDC_SIZEWE,
			IMAGE_CURSOR,
			0,
			0,
			LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_SHARED));
	}

	template <class Derived, class Traits>
	object_processor<Derived, Traits>::~object_processor()
	{
		::DeleteObject(hCursor[0]);
		::DeleteObject(hCursor[1]);
		::DeleteObject(hCursor[2]);
		::DeleteObject(hCursor[3]);
	}

	template <class Derived, class Traits>
	bool object_processor<Derived, Traits>::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &l)
	{
		using object = typename Traits::object;
		using point = typename Traits::point;
		using creation_params = typename Traits::creation_params;

		switch(msg){
		case WM_MOUSEMOVE:
			{
				inside = true;

				auto ht = object::create_hittest(static_cast<Derived*>(this));
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				if(pushing && focus != nullptr){
					focus->on_mouse_move(point(x, y), ht);
				}else{
					if(hover != nullptr && !hover->is_colliding(point(x, y))){
						hover->on_mouse_leave(ht);
						hover = nullptr;
					}
					static_cast<Derived*>(this)->iterate_objects([&](typename object &o)-> bool{
						if(o.is_colliding(point(x, y))){
							o.on_mouse_move(point(x, y), ht);
							if(hover != nullptr && &o != hover){
								hover->on_mouse_leave(ht);
							}
							hover = &o;
							return true;
						}
						return false;
					});
				}

				set_cursor_type(static_cast<cursor_type>(static_cast<int>(ht.cursor_)));

				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = static_cast<Derived*>(this)->get_hwnd();
				::TrackMouseEvent(&tme);
			}
			break;
		case WM_MOUSELEAVE:
			inside = false;
			if(hover != nullptr){
				auto ht = object::create_hittest(static_cast<Derived*>(this));
				hover->on_mouse_leave(ht);
				hover = nullptr;
				set_cursor_type(static_cast<cursor_type>(static_cast<int>(ht.cursor_)));
			}
			break;
		case WM_LBUTTONDOWN:
			{
				auto ht = object::create_hittest(static_cast<Derived*>(this));
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				bool collide = false;

				pushing = true;

				static_cast<Derived*>(this)->iterate_objects([&](typename object &o)-> bool{
					if(o.is_colliding(point(x, y))){
						if(focus != nullptr && &o != focus){
							focus->on_lose_focus(ht);
						}
						o.on_left_press(point(x, y), ht);
						focus = &o;
						collide = true;
						return true;
					}
					return false;
				});
				if(!collide && focus != nullptr){
					focus->on_lose_focus(ht);
					focus = nullptr;
				}

				set_cursor_type(static_cast<cursor_type>(static_cast<int>(ht.cursor_)));
				::SetCapture(static_cast<Derived*>(this)->get_hwnd());
			}
			break;
		case WM_LBUTTONUP:
			{
				auto ht = object::create_hittest(static_cast<Derived*>(this));
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				pushing = false;

				if(focus != nullptr){
					focus->on_left_release(point(x, y), ht);
				}
				
				set_cursor_type(static_cast<cursor_type>(static_cast<int>(ht.cursor_)));
				::ReleaseCapture();
			}
			break;
		case WM_KILLFOCUS:
			if(focus != nullptr){
				focus->on_lose_focus(object::create_hittest(static_cast<Derived*>(this)));
				focus = nullptr;
			}
			break;
		case WM_SETCURSOR:
			if(inside){
				::SetCursor(hCursor[cursortype]);
				l = TRUE;
				return false;
			}
			break;
		}

		return true;
	}

} }
