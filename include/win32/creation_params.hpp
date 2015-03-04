#pragma once

#include <Windows.h>

namespace quote{ namespace win32{

	struct creation_params{
		DWORD exstyle, style;
		const wchar_t *classname, *title;
		int x, y, w, h;
		HWND hparent;
		void *data;
	};

} }
