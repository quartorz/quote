#pragma once

#include <cef_base.h>
#include <cef_app.h>

namespace quote{ namespace cef{

	template <typename Derived>
	class app : public CefApp, public CefBrowserProcessHandler{
		CefRefCount refcount_;

	public:
		void AddRef() const override
		{
			refcount_.AddRef();
		}

		bool Release() const override
		{
			return refcount_.Release();
		}

		bool HasOneRef() const override
		{
			return refcount_.HasOneRef();
		}

		CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
		{
			return this;
		}

		// CefBrowserProcessHandler methods:
		void OnContextInitialized() override
		{
			static_cast<Derived*>(this)->on_cef_context_initialized();
		}

		void on_cef_context_initialized()
		{
		}
	};

} }
