#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"

namespace quote{ namespace cef{

	template <typename Derived>
	class renderer_handler :
		public CefApp,
		public CefRenderProcessHandler
	{
		IMPLEMENT_REFCOUNTING(Derived);

	public:
		CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
		{
			return this;
		}

		// CefRenderProcessHandler
		void OnContextCreated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context
		) override
		{
			static_cast<Derived*>(this)->on_cef_context_created(browser, frame, context);
		}

		void on_cef_context_created(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context
		)
		{
			// dummy method
		}

		bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message
		) override
		{
			MessageBoxW(0, L"message received", L"", 0);
			return static_cast<Derived*>(this)->on_cef_render_process_message_received(
				browser,
				source_process,
				message);
		}

		bool on_cef_render_process_message_received(
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message
		)
		{
			// dummy method
			return false;
		}
	};

} }
