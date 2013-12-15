#pragma once

#include <Windows.h>

#include <string>

inline std::wstring get_directory()
{
	wchar_t path[_MAX_PATH];
	::GetModuleFileNameW(nullptr, path, _MAX_PATH);
	::wcsrchr(path, L'\\')[1] = L'\0';
	return path;
}
