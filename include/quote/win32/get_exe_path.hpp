#pragma once

#include <Windows.h>

#include <string>
#include <cstdint>
#include <cstdlib>
#include <cassert>

namespace quote{ namespace win32{

	inline std::wstring get_exe_path()
	{
		wchar_t path[MAX_PATH];
		auto length = ::GetModuleFileNameW(::GetModuleHandleW(nullptr), path, _countof(path));

		assert(length <= _countof(path));

		return path;
	}

} }
