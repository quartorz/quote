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

		void draw_line(const paint_params &pp, const brush &brush, line line);
		void draw_line(const paint_params &pp, const brush *brush, const line &line)
		{
			draw_line(pp, *brush, line);
		}
		void draw_rect(const paint_params &pp, const brush &brush, const rect &rect);
		void draw_rect(const paint_params &pp, const brush *brush, const rect &rect)
		{
			draw_rect(pp, *brush, rect);
		}
		void draw_circle(const paint_params &pp, const brush &brush, circle circle);
		void draw_circle(const paint_params &pp, const brush *brush, const circle &circle)
		{
			draw_circle(ps, brush, circle);
		}
		void draw_text(const paint_params &pp, const font &font, const brush &brush, const wchar_t *text, const rect &rect, bool clip=false);
		void draw_text(const paint_params &pp, const font *font, const brush &brush, const wchar_t *text, const rect &rect, bool clip=false)
		{
			draw_text(pp, *font, brush, text, rect, clip);
		}
		void draw_text(const paint_params &pp, const font &font, const brush *brush, const wchar_t *text, const rect &rect, bool clip=false)
		{
			draw_text(pp, font, *brush, text, rect, clip);
		}
		void draw_text(const paint_params &pp, const font *font, const brush *brush, const wchar_t *text, const rect &rect, bool clip=false)
		{
			draw_text(pp, *font, *brush, text, rect, clip);
		}
		void clear(const paint_params &, const color &);

		void repaint()
		{
			::InvalidateRect(get_hwnd(), nullptr, FALSE);
		}

		bool CreateResource();
		void DestroyResource();
		bool WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &r);
	};

} }

#include "details/painter.hpp"
