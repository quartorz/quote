#pragma once

#if !defined(DIRECTINPUT_VERSION)
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#include "utils.hpp"

#pragma comment(lib, "Dinput8")
#pragma comment(lib, "dxguid")

#include <vector>
#include <tuple>
#include <string>
#include <climits>

namespace quote{ namespace win32{

	template <class Derived, UINT_PTR TimerID=UINT_PTR_MAX>
	class joystick{
		using device_type = std::tuple<GUID, LPDIRECTINPUTDEVICE8>;
		std::vector<device_type> devices;

		void load_devices(HWND);
		void release_devices();

	public:
		~joystick();

		bool has_joystick() const;
		DIDEVCAPS get_joystick_capabilities(GUID &) const;
		bool WindowProc(HWND, UINT, WPARAM, LPARAM, LRESULT &);
	};

} }

#include "impl/joystick.hpp"
