#pragma once

#include "include/cef_browser.h"

namespace quote{ namespace cef{

	namespace detail{
		template <typename Func>
		class StringVisitor : public CefStringVisitor {
			IMPLEMENT_REFCOUNTING(StringVisitor<Func>);

			Func f_;

		public:
			StringVisitor(Func f)
				: f_(f)
			{
			}

			void Visit(const CefString &s) override
			{
				f_(s);
			}
		};
	}

	template <typename Func>
	CefRefPtr<detail::StringVisitor<Func>> make_string_visitor(Func f)
	{
		return new detail::StringVisitor<Func>(f);
	}

} }
