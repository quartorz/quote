#pragma once

#include <quote/direct2d/painter.hpp>

#include <VersionHelpers.h>

namespace quote{ namespace direct2d{

	namespace detail{
		struct ACCENTPOLICY{
			int nAccentState;
			int nAccentFlags;
			int nGradientColor;
			int nAnimationId;
		};

		struct WINDOWCOMPOSITIONATTRIBUTEDATA{
			int nAttribute;
			LPVOID pData;
			ULONG ulDataSize;
		};
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
	bool painter<Derived>::WindowProc(Derived &, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &r)
	{
		switch(msg){
		case WM_CREATE:
			{
				/*if(::IsWindowsVersionOrGreater(6, 2, 0)){
					HINSTANCE module = ::LoadLibrary(L"user32.dll");

					if(module == nullptr)
						break;

					using lpfnSetWindowCompositionAttribute = BOOL(WINAPI *)(HWND, detail::WINDOWCOMPOSITIONATTRIBUTEDATA*);
					auto SetWindowCompositionAttribute = (lpfnSetWindowCompositionAttribute)::GetProcAddress(module, "SetWindowCompositionAttribute");

					if(SetWindowCompositionAttribute == nullptr){
						::FreeLibrary(module);
						break;
					}

					detail::ACCENTPOLICY policy = {3, 0, 0, 0};
					detail::WINDOWCOMPOSITIONATTRIBUTEDATA data = {19, &policy, sizeof(policy)};

					SetWindowCompositionAttribute(hwnd, &data);

					::FreeLibrary(module);
				}else{
					DWM_BLURBEHIND bb = {};
					bb.dwFlags = DWM_BB_ENABLE;
					bb.fEnable = TRUE;
					::DwmEnableBlurBehindWindow(
						get_hwnd(),
						&bb);
					htheme = ::OpenThemeData(hwnd, VSCLASS_WINDOW);

					BOOL b;
					::DwmIsCompositionEnabled(&b);
					aero_glass = b != FALSE;
				}*/
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

				if(!aero_glass)
					target->Clear(color(255, 255, 255, 255));

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
		case WM_DWMCOMPOSITIONCHANGED:
			{
				BOOL b;
				::DwmIsCompositionEnabled(&b);
				aero_glass = b != FALSE;
			}
			break;
		}
		return true;
	}

} }
