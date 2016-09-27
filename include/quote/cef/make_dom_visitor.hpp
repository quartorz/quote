#pragma once

#include "include/cef_browser.h"

namespace quote{ namespace cef{

	namespace detail{
		template <typename Func>
		class DOMVisitor : public CefDOMVisitor {
			IMPLEMENT_REFCOUNTING(DOMVisitor<Func>);

			Func f_;

		public:
			DOMVisitor(Func f)
				: f_(f)
			{
			}

			void Visit(CefRefPtr<CefDOMDocument> document) override
			{
				f_(document);
			}
		};
	}

	template <typename Func>
	CefRefPtr<detail::DOMVisitor<Func>> make_dom_visitor(Func f)
	{
		return new detail::DOMVisitor<Func>(f);
	}

} }
