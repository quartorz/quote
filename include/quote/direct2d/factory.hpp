#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <comdef.h>

namespace{
	struct D2D{
	}D2D;
	struct DWrite{
	}DWrite;
	struct WIC{
	}WIC;
}

namespace quote{ namespace direct2d{

	class factory{
		static ID2D1Factory *d2dfactory;
		static IDWriteFactory *dwfactory;
		static IWICImagingFactory *wicfactory;
	public:
		struct Init{
		};

		factory()
		{
			d2dfactory->AddRef();
			dwfactory->AddRef();
			wicfactory->AddRef();
		}
		factory(Init&)
		{
			using namespace _com_util;
			CheckError(::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dfactory));
			CheckError(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwfactory), reinterpret_cast<IUnknown**>(&dwfactory)));
			CheckError(::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicfactory)));
		}
		factory(const factory &)
		{
			d2dfactory->AddRef();
			dwfactory->AddRef();
			wicfactory->AddRef();
		}
		~factory()
		{
			d2dfactory->Release();
			dwfactory->Release();
			wicfactory->Release();
		}
		ID2D1Factory *operator[](struct D2D&) const
		{
			return d2dfactory;
		}
		IDWriteFactory *operator[](struct DWrite&) const
		{
			return dwfactory;
		}
		IWICImagingFactory *operator[](struct WIC&) const
		{
			return wicfactory;
		}
	};

	extern factory *_factory;

} }
