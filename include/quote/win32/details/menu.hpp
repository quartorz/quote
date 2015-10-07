#pragma once

#include "../menu.hpp"

namespace quote{ namespace win32{

	bool menu::create()
	{
		if(handle != nullptr)
			return false;

		handle = ::CreateMenu();
		primary = true;

		return handle != nullptr;
	}

	bool menu::create_popup()
	{
		if(handle != nullptr)
			return false;

		handle = ::CreatePopupMenu();
		primary = false;
		popup = true;

		return handle != nullptr;
	}

	menu *menu::add_menu(int position, const wchar_t *caption, bool has_child /* =false */)
	{
		auto m = add_menu(position, menu_id, caption, has_child);
		if(m != nullptr)
			menu_id++;
		return m;
	}

	menu *menu::add_menu(int position, unsigned id, const wchar_t *caption, bool has_child /* =false */)
	{
		MENUITEMINFOW mii = {};
		mii.cbSize = sizeof mii;
		mii.fMask = MIIM_STRING | MIIM_SUBMENU | MIIM_ID | MIIM_DATA | MIIM_FTYPE;
		mii.dwItemData = reinterpret_cast<ULONG_PTR>(new menu);
		mii.dwTypeData = const_cast<LPWSTR>(caption);
		if(has_child)
			mii.hSubMenu = ::CreatePopupMenu();
		mii.wID = id;
		if(!::InsertMenuItemW(handle, position, TRUE, &mii)){
			delete reinterpret_cast<menu*>(mii.dwItemData);
			return nullptr;
		}
		mii.fMask = MIIM_SUBMENU;
		::GetMenuItemInfoW(handle, position, TRUE, &mii);
		auto ret = reinterpret_cast<menu*>(mii.dwItemData);
		ret->handle = mii.hSubMenu;
		ret->id = id;
		ret->caption = caption;
		if(primary)
			ret->popup = false;
		else
			ret->popup = true;
		ret->has_child = has_child;
		return ret;
	}

	menu *menu::get_menu(int id_or_pos, bool by_pos /* =false */)
	{
		if(by_pos){
			MENUITEMINFOW mii;
			mii.cbSize = sizeof mii;
			mii.fMask = MIIM_DATA;
			::GetMenuItemInfoW(handle, id, TRUE, &mii);
			return reinterpret_cast<menu*>(mii.dwItemData);
		}else{
			menu *r = nullptr;
			int c = ::GetMenuItemCount(handle);
			MENUITEMINFOW mii;
			mii.cbSize = sizeof mii;
			mii.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_ID;
			for(int i = 0; i < c; ++i){
				::GetMenuItemInfoW(handle, i, TRUE, &mii);
				if(mii.wID == id_or_pos)
					return reinterpret_cast<menu*>(mii.dwItemData);
				else{
					r = reinterpret_cast<menu*>(mii.dwItemData)->get_menu(id_or_pos);
					if(r != nullptr)
						return r;
				}
			}
			return nullptr;
		}
	}

	void menu::delete_all_children()
	{
		int c = ::GetMenuItemCount(handle);
		MENUITEMINFOW mii;
		mii.cbSize = sizeof mii;
		mii.fMask = MIIM_SUBMENU | MIIM_DATA;
		for(int i = 0; i < c; ++i){
			::GetMenuItemInfoW(handle, 0, TRUE, &mii);
			auto m = reinterpret_cast<menu*>(mii.dwItemData);
			m->delete_all_children();
			delete m;
			::DeleteMenu(handle, 0, MF_BYPOSITION);
		}
	}

	int menu::get_child_count() const
	{
		return ::GetMenuItemCount(handle);
	}

	template <class Window>
	void menu::show_popup(const Window &w)
	{
		POINT pt;
		::GetCursorPos(&pt);
		return ::TrackPopupMenuEx(
			hmenu,
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
			pt.x,
			pt.y,
			w.get_hwnd(),
			NULL);
	}

	void menu::set_handler(const std::function<void(menu&)> &h)
	{
		handler = h;
	}

	void menu::on_select()
	{
		handler(*this);
	}

} }
