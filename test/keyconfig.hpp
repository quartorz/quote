#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <Windows.h>

#if !defined DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#include "singleton.hpp"

class keyconfig: public singleton<keyconfig>{
	friend singleton<keyconfig>;

public:
	enum class button{
		up,
		down,
		left,
		right,
		a,
		b,
		c,
		menu,
	};
	struct State{
		bool up, down, left, right, a, b, c, menu;
	};

	static const int button_count = 8;
	
private:
	struct JoystickButton{
		enum class Tag{
			None,
			Axis0_Positive,
			Axis0_Negative,
			Axis1_Positive,
			Axis1_Negative,
			Axis2_Positive,
			Axis2_Negative,
			Rot0_Positive,
			Rot0_Negative,
			Rot1_Positive,
			Rot1_Negative,
			Rot2_Positive,
			Rot2_Negative,
			Slider0_Positive,
			Slider0_Negative,
			Slider1_Positive,
			Slider1_Negative,
			POV0_Up,
			POV0_Down,
			POV0_Left,
			POV0_Right,
			POV1_Up,
			POV1_Down,
			POV1_Left,
			POV1_Right,
			POV2_Up,
			POV2_Down,
			POV2_Left,
			POV2_Right,
			POV3_Up,
			POV3_Down,
			POV3_Left,
			POV3_Right,
			Button0,
		}tag;
	};
	const wchar_t *to_string(JoystickButton::Tag);
	const wchar_t *to_string(unsigned);

	GUID js_id;
	JoystickButton js_config[8];
	unsigned kb_config[8];
	unsigned char state[8];

	keyconfig();

public:
	keyconfig(const keyconfig&) = delete;
	keyconfig &operator=(const keyconfig&) = delete;

	~keyconfig();

	State get_state() const;
	void reset();
	const GUID &get_joystick_id() const
	{
		return js_id;
	}
	const wchar_t *get_keyboard_config(button) const;
	const wchar_t *get_joystick_config(button) const;
	const wchar_t *set_keyboard_config(button, unsigned);
	const wchar_t *set_joystick_config(button, const GUID &, const DIJOYSTATE2 &);
	void set_keyboard_state(unsigned, bool push);
	void set_joystick_state(const GUID &, const DIJOYSTATE2 &);
};
