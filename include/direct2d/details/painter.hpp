#pragma once

#include "../painter.hpp"

namespace quote{ namespace direct2d{

	template <class Derived>
	inline void painter<Derived>::draw_line(const paint_params &pp, const brush &brush, line line)
	{
		pp.target->DrawLine(line.GetStartPoint(), line.GetEndPoint(), brush.Get());
	}

	template <class Derived>
	inline void painter<Derived>::draw_rect(const paint_params &pp, const brush &brush, const rect &rect)
	{
		pp.target->FillRectangle(
			rect,
			brush.Get());
	}

	template <class Derived>
	inline void painter<Derived>::draw_circle(const paint_params &pp, const brush &brush, circle circle)
	{
		pp.target->DrawEllipse(
			D2D1::Ellipse(circle.GetCenter(), circle.GetRadius(), circle.GetRadius()),
			brush.Get());
	}

	template <class Derived>
	inline void painter<Derived>::draw_text(const paint_params &pp, const font &font, const brush &brush, const wchar_t *text, const rect &rect, bool clip)
	{
		pp.target->DrawTextW(
			text,
			std::wcslen(text),
			font.Get(),
			rect,
			brush.Get(),
			(clip ? D2D1_DRAW_TEXT_OPTIONS_CLIP : D2D1_DRAW_TEXT_OPTIONS_NONE));
	}

	template <class Derived>
	void painter<Derived>::clear(const paint_params &pp, const color &c)
	{
		pp.target->Clear(c);
	}

	template <class Derived>
	inline bool painter<Derived>::CreateResource()
	{
		if(target != nullptr)
			return true;

		auto props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED),
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT);
		if(FAILED(factory[D2D]->CreateDCRenderTarget(
			&props,
			&target)))
			return false;

		return static_cast<Derived*>(this)->create_resource(creation_params());
	}

	template <class Derived>
	inline void painter<Derived>::DestroyResource()
	{
		static_cast<Derived*>(this)->destroy_resource();
		::quote::utils::SafeRelease(target);
	}

	template <class Derived>
	bool painter<Derived>::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &r)
	{
		switch(msg){
		case WM_CREATE:
			{
				DWM_BLURBEHIND bb ={};
				bb.dwFlags = DWM_BB_ENABLE;
				bb.fEnable = TRUE;
				::DwmEnableBlurBehindWindow(
					hwnd,
					&bb);
				htheme = ::OpenThemeData(hwnd, VSCLASS_WINDOW);
			}
			break;
		case WM_DESTROY:
			::CloseThemeData(htheme);
			DestroyResource();
			break;
		case WM_PAINT:
			if(CreateResource()){
				mutex.lock();

				paint_params pp = {
					target
				};

				HWND hwnd = get_hwnd();
				RECT rc;
				::GetClientRect(hwnd, &rc);

				PAINTSTRUCT ps;
				::BeginPaint(hwnd, &ps);

				target->BindDC(ps.hdc, &rc);
				target->BeginDraw();

				static_cast<Derived*>(this)->draw(pp);

				if(target->EndDraw() == D2DERR_RECREATE_TARGET){
					DestroyResource();
					repaint();
				}

				::EndPaint(hwnd, &ps);

				mutex.unlock();
			}else{
				throw std::runtime_error("an error occurred when creating Direct2D resources.");
			}
			break;
		case WM_SIZE:
			::InvalidateRect(hwnd, nullptr, FALSE);
			break;
		}
		return true;
	}

} }
