#pragma once

#include "../joystick.hpp"
#include "../utils.hpp"

#include <comdef.h>

#include <functional>
#include <algorithm>

namespace quote{ namespace win32{

	template <class Derived, UINT_PTR TimerID>
	void joystick<Derived, TimerID>::load_devices(HWND hwnd)
	{
		if(devices.size() != 0)
			return;

		using namespace _com_util;

		_COM_SMARTPTR_TYPEDEF(IDirectInput8W, IID_IDirectInput8W);
		IDirectInput8WPtr input;
		CheckError(::DirectInput8Create(::GetModuleHandleW(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8W, reinterpret_cast<LPVOID*>(&input), nullptr));

		std::vector<std::tuple<GUID, std::wstring>> joysticks;

		LPVOID context[] = {input, &devices, &joysticks};
		input->EnumDevices(DI8DEVCLASS_GAMECTRL, [](const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)->BOOL{
			// std::wcerr << L"instance name: " << pdidInstance->tszInstanceName << std::endl;
			// std::wcerr << L"product name: " << pdidInstance->tszProductName << std::endl << std::endl;

			auto context = static_cast<LPVOID*>(pContext);
			auto input = static_cast<LPDIRECTINPUT8>(context[0]);
			auto devices = static_cast<std::vector<device_type>*>(context[1]);
			auto joysticks = static_cast<std::vector<std::tuple<GUID, std::wstring>>*>(context[2]);
			LPDIRECTINPUTDEVICE8 device;

			CheckError(input->CreateDevice(
				pdidInstance->guidInstance,
				&device,
				nullptr));

			devices->emplace_back(pdidInstance->guidInstance, device);
			joysticks->emplace_back(pdidInstance->guidInstance, pdidInstance->tszInstanceName);

			return DIENUM_CONTINUE;
		}, context, DIEDFL_ATTACHEDONLY);

		for(auto &tuple: devices){
			LPDIRECTINPUTDEVICE8 device = std::get<1>(tuple);

			CheckError(device->SetDataFormat(&c_dfDIJoystick2));

			device->EnumObjects([](LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID pvRef)->BOOL{
				auto device = static_cast<LPDIRECTINPUTDEVICE>(pvRef);

				DIPROPRANGE diprg;

				diprg.diph.dwSize = sizeof(DIPROPRANGE);
				diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				diprg.diph.dwHow = DIPH_BYID;
				diprg.diph.dwObj = pdidoi->dwType;
				diprg.lMin = -32768;
				diprg.lMax = 32767;

				if(FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
					return DIENUM_STOP;

				return DIENUM_CONTINUE;
			}, device, DIDFT_AXIS);
		}

		for(auto &tuple : devices){
			_com_util::CheckError(std::get<1>(tuple)->SetCooperativeLevel(
				hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND));
		}

		static_cast<Derived*>(this)->on_reload_joysticks(joysticks);
	}

	template <class Derived, UINT_PTR TimerID>
	void joystick<Derived, TimerID>::release_devices()
	{
		for(auto &tuple: devices){
			::quote::utils::SafeRelease(std::get<1>(tuple));
		}
		devices.clear();
	}

	template <class Derived, UINT_PTR TimerID>
	joystick<Derived, TimerID>::~joystick()
	{
		release_devices();
	}

	template <class Derived, UINT_PTR TimerID>
	bool joystick<Derived, TimerID>::has_joystick() const
	{
		return devices.size() != 0;
	}

	template <class Derived, UINT_PTR TimerID>
	DIDEVCAPS joystick<Derived, TimerID>::get_joystick_capabilities(GUID &guid) const
	{
		auto it = std::find_if(devices.begin(), devices.end(), [&guid](device_type &d){return std::get<0>(d) == guid;});
		if(it != devices.end()){
			DIDEVCAPS diDevCaps;
			diDevCaps.dwSize = sizeof(DIDEVCAPS);
			std::get<1>(*it)->GetCapabilities(&diDevCaps);
			return diDevCaps;
		}

		throw std::runtime_error("joystick not found");
	}

	template <class Derived, UINT_PTR TimerID>
	bool joystick<Derived, TimerID>::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &lresult)
	{
		switch(msg){
		case WM_CREATE:
			load_devices(hwnd);
			break;
		case WM_DESTROY:
			release_devices();
			break;
		case WM_TIMER:
			if(wParam == TimerID){
				for(auto &tuple: devices){
					auto device = std::get<1>(tuple);

					if(FAILED(device->Poll())){
						while(device->Acquire() == DIERR_INPUTLOST);
						return true;
					}

					DIJOYSTATE2 js;
					if(FAILED(device->GetDeviceState(sizeof(DIJOYSTATE2), &js)))
						return true;

					static_cast<Derived*>(this)->on_get_joystick_state(std::get<0>(tuple), js);
				}
			}
			break;
		case WM_DEVICECHANGE:
			release_devices();
			load_devices(hwnd);
			break;
		}

		return true;
	}

} }
