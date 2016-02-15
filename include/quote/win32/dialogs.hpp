#pragma once

#include <Windows.h>
#include <malloc.h>

#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cstdint>

#include <quote/tmp/on_exit.hpp>

namespace quote{ namespace win32{

	template <class Window>
	// return: tuple<files, index of filter>
	inline std::tuple<std::vector<std::wstring>, std::uint32_t> get_open_file(const Window &parent, const std::vector<std::pair<std::wstring, std::vector<std::wstring>>> &filter = {}, std::uint32_t index = 1)
	{
		std::vector<wchar_t> f;
		auto inserter = std::back_inserter(f);

		if(filter.size() != 0){
			for(auto &p: filter){
				auto &label = std::get<0>(p);
				auto &ext = std::get<1>(p);

				auto add_extension = [&inserter, &ext](){
					for(auto &e: ext){
						*inserter = L'*';
						std::copy(e.begin(), e.end(), inserter);
						*inserter = L';';
					}
				};

				if(ext.size() == 0)
					continue;

				std::copy(label.begin(), label.end(), inserter);
				*inserter = L' ';
				*inserter = L'(';

				add_extension();
				f.back() = L')';
				*inserter = L'\0';

				add_extension();
				f.back() = L'\0';
			}
		}else{
			const wchar_t *s = L"All Files (*.*)\0*.*\0";
			f = std::vector<wchar_t>(s, s + 20);
		}
		*inserter = L'\0';

		std::vector<std::wstring> result;

		DWORD length = ::GetCurrentDirectoryW(0, nullptr);
		auto currentdir = reinterpret_cast<wchar_t*>(_malloca(sizeof(wchar_t) * length));
		::GetCurrentDirectoryW(length, currentdir);

		auto buffer = reinterpret_cast<wchar_t*>(_malloca(sizeof(wchar_t) * _MAX_PATH * 10000));
		buffer[0] = 0;

		QUOTE_ON_EXIT{
			_freea(currentdir);
			_freea(buffer);
		};

		OPENFILENAMEW ofn = {};
		ofn.lStructSize = sizeof ofn;
		ofn.hwndOwner = parent.get_hwnd();
		ofn.hInstance = ::GetModuleHandleW(nullptr);
		ofn.lpstrFilter = &f[0];
		ofn.lpstrCustomFilter = nullptr;
		ofn.nMaxCustFilter = 1;
		ofn.nFilterIndex = index;
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = _MAX_PATH * 10000;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = currentdir;
		ofn.lpstrTitle = nullptr;
		ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = L"png";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;

		if(filter.size() != 0){
			if(std::get<1>(filter[0]).size() != 0){
				ofn.lpstrDefExt = &std::get<1>(filter[0])[0][1];
			}
		}

		if(::GetOpenFileNameW(&ofn)){
			wchar_t *iter = std::wcschr(buffer, L'\0');
			if(*++iter == L'\0'){
				result.emplace_back(buffer);
			}else{
				do{
					result.emplace_back(buffer);
					result.back() += L'\\';
					result.back() += iter;

					iter = ::wcschr(iter, L'\0') + 1;
				}while(*iter != L'\0');
			}
		}

		return std::make_tuple(result, ofn.nFilterIndex);
	}

	inline std::wstring get_save_file()
	{
	}

} }
