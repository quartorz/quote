#include <quote/quote.hpp>
#include <quote/win32/message_loop.hpp>

#include "main_window.hpp"

int run()
{
	if(!main_window::register_class())
		return 0;

	main_window window;
	if(!window.create(nullptr, L"Sample", INT_MAX, INT_MAX, 800, 600))
		return 0;
	window.show();

	return window_system::message_loop(window_system::make_frame_controller<100>(window));
}

QUOTE_DEFINE_MAIN
