#pragma once

#include <d2d1.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <vsstyle.h>

#include "utils.hpp"
#include "factory.hpp"
#include "base_types.hpp"
#include "font.hpp"

#include <mutex>

namespace quote{ namespace direct2d{

	template <class Derived>
	class painter{
		inline HWND get_hwnd()
		{
			return static_cast<Derived*>(this)->get_hwnd();
		}

		factory factory;
		HTHEME htheme;
		ID2D1DCRenderTarget *target;
		std::recursive_mutex mutex;
		bool aero_glass;

	public:
		using paint_params = paint_params;

		quote::direct2d::creation_params creation_params()
		{
			quote::direct2d::creation_params cp = {
				target
			};
			return cp;
		}

		painter():
			htheme(nullptr),
			target(nullptr)
		{
		}
		~painter()
		{
			::quote::utils::SafeRelease(target);
		}

		bool aero_glass_enabled() const
		{
			return aero_glass;
		}

		void repaint()
		{
			::InvalidateRect(get_hwnd(), nullptr, FALSE);
		}

		bool CreateResource();
		void DestroyResource();
		bool WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &r);
	};

} }

#include "impl/painter.hpp"
