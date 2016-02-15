#include "quote/quote.hpp"

#include "quote/win32/window.hpp"
#include "quote/win32/procs.hpp"
#include "quote/win32/async_message_loop.hpp"
#include "quote/win32/object_processor.hpp"

#include "quote/direct2d/traits.hpp"
#include "quote/direct2d/painter.hpp"
#include "quote/direct2d/rect_object.hpp"
#include "quote/direct2d/object_manager.hpp"

class main_window : 
	public quote::win32::window<
		main_window, 
		quote::win32::quit_on_close<main_window>,
		quote::win32::object_processor<main_window, quote::direct2d::traits>,
		quote::direct2d::painter<main_window>>,
	public quote::win32::async_message_loop<main_window>,
	public quote::direct2d::object_manager<main_window>
{
	quote::direct2d::rect_object r;
public:
	static const wchar_t *get_class_name()
	{
		return L"main_window";
	}
	bool initialize()
	{
		r.right = 100;
		r.bottom = 100;

		register_object(&r);

		return true;
	}
	void uninitialize()
	{
		unregister_object(&r);
	}
	void draw(const quote::direct2d::paint_params &pp)
	{
		pp.clear({255, 255, 255, 255});
		object_manager<main_window>::draw(pp);
	}
};

int run()
{
	if(!main_window::register_class())
		return 0;

	main_window w;

	w.create(nullptr, L"title");
	w.show();
	w.repaint();

	w.join_message_loop();

	return 0;
}

QUOTE_DEFINE_MAIN
