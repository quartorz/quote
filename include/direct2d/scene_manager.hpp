#pragma once

#include "../scene_manager.hpp"
#include "scene.hpp"
#include "traits.hpp"

#if !defined DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

namespace quote{ namespace direct2d{

	template <class Derived, bool MultiThread = false>
	class scene_manager: public ::quote::scene_manager<Derived, traits, MultiThread>{
		using scene_type = scene<MultiThread>;

	protected:
		using joystick_id = GUID;
		using joystick_state = DIJOYSTATE2;

	public:
		void on_get_joystick_state(const GUID &guid, const DIJOYSTATE2 &js)
		{
			auto scene = this->get_current_scene();
			static_cast<scene_type*>(scene)->on_get_joystick_state(guid, js);
		}

		void on_reload_joysticks(const std::vector<std::tuple<joystick_id, std::wstring>> &joysticks)
		{
			for(auto &scene: scenes){
				static_cast<scene_type*>(std::get<1>(scene))->on_reload_joysticks(joysticks);
			}
		}
	};

} }
