#include <quote/quote.hpp>
#include <quote/win32/message_loop.hpp>
#include <quote/cef/prepare.hpp>
#include <quote/cef/wrap.hpp>
#include <quote/cef/app.hpp>

#include "main_window.hpp"

#if defined(NDEBUG)
#  pragma comment(lib, "lib\\Release\\cef_sandbox.lib")
#  pragma comment(lib, "lib\\Release\\libcef.lib")
#  pragma comment(lib, "lib\\Release\\libcef_dll_wrapper.lib")
#else
#  pragma comment(lib, "lib\\Debug\\cef_sandbox.lib")
#  pragma comment(lib, "lib\\Debug\\libcef.lib")
#  pragma comment(lib, "lib\\Debug\\libcef_dll_wrapper.lib")
#endif

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler {
 public:
  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE {}

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

int run()
{
	int exit_code;
	void *sandbox_info = nullptr;

	if (quote::cef::prepare(exit_code, sandbox_info)) {
		return exit_code;
	}

	if (!main_window::register_class())
		return 0;

	main_window window;

	if (!window.create(nullptr, L"CEF sample"))
		return 0;

	window.show();

	return quote::cef::wrap(
		&window,
		sandbox_info,
		[]() {
			return quote::win32::message_loop(
				quote::cef::message_handler()
			);
		}
	);
}

QUOTE_DEFINE_MAIN
