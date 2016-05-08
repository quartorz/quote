#pragma once

#include "include/cef_command_line.h"

namespace quote{ namespace cef{

	cef_process_id_t process_type(const wchar_t *command_line_str)
	{
		auto command_line = CefCommandLine::CreateCommandLine();
		command_line->InitFromString(command_line_str);

		if (!command_line->HasSwitch("type")) {
			return PID_BROWSER;
		}

		auto type = command_line->GetSwitchValue("type");

		if (type == "renderer") {
			return PID_RENDERER;
		}

		// Unknown process type
		return static_cast<cef_process_id_t>(-1);
	}

	auto process_type()
	{
		return process_type(::GetCommandLineW());
	}

} }
