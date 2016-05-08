#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"

namespace quote{ namespace cef{

	template <typename Derived>
	class browser_handler :
		public CefApp,
		public CefBrowserProcessHandler
	{
	public:
		CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
		{
			return this;
		}
	};

} }
