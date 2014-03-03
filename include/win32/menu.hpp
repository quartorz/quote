#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <functional>
#include <set>

namespace quote{ namespace win32{

	class menu{
		static unsigned menu_id;

		HMENU handle;
		unsigned id;
		std::wstring caption;
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
						delete reinterpret_cast<menu*>(mii.dwItemData);
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
		void show_popup(const Window &w);
		void set_handler(const std::function<void(menu&)> &);

		void on_select();
	};

	__declspec(selectany) unsigned menu::menu_id = 100;

	template <class Derived>
	class menu_proccessor{
		std::set<menu*> menus;

	public:
		void register_menu(menu &m)
		{
			register_menu(&m);
		}
		void register_menu(menu *m)
		{
			menus.insert(m);
		}
		void unregister_menu(menu &m)
		{
			unregister_menu(&m);
		}
		void unregister_menu(menu *m)
		{
			menus.erase(m);
		}
		bool WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
		{
			switch(msg){
			case WM_COMMAND:
				if(GET_WM_COMMAND_CMD(wParam, lParam) == 0 /* menu */){
					auto id = GET_WM_COMMAND_ID(wParam, lParam);
					for(auto &m: menus){
						auto menu = m->get_menu(id);
						if(menu != nullptr){
							menu->on_select();
							break;
						}
					}
				}
			}

			return true;
		}
	};

} }

#include "details/menu.hpp"
