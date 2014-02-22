#pragma once

#include <Windows.h>

#include <functional>

namespace quote{ namespace win32{

	class menu{
		static unsigned menu_id = 0;

		HMENU handle;
		unsigned id;
		bool primary, popup, has_child;

		std::function<void(menu&)> handler;

	public:
		HMENU get_handle() const
		{
			return handle;
		}

		menu(HMENU handle = nullptr):
			handle(handle), primary(false), popup(false), has_child(false)
		{
		}
		~menu()
		{
			if(handle != nullptr){
				int c = ::GetMenuItemCount(handle);
				MENUITEMINFO mii;
				mii.cbSize = sizeof mii;
				mii.fMask = MIIM_FTYPE | MIIM_DATA;
				for(int i = 0; i < c; ++i){
					::GetMenuItemInfoW(handle, i, TRUE, &mii);
					if(mii.dwItemData != NULL)
						delete reinterpret_cast<Derived*>(mii.dwItemData);
				}
				::DestroyMenu(handle);
			}
		}

		bool create();
		bool create_popup();
		menu *add_menu(int position, const wchar_t *caption, bool has_child = false);
		menu *add_menu(int position, unsigned id, const wchar_t *caption, bool has_child = false);
		menu *get_menu(int id_or_pos, bool by_pos = false);
		void delete_all_children();
		int get_child_count() const;
		template <class Window>
		void show_popup();
	};

	__declspec(selectany) unsigned menu::menu_id = 1;

} }

#include "details/menu.hpp"
