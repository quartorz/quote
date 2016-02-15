#include <stdexcept>

#include <quote/direct2d/factory.hpp>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dwmapi")
#pragma comment(lib, "uxtheme")

namespace quote{ namespace direct2d{

	__declspec(selectany) factory *_factory = nullptr;

	__declspec(selectany) ID2D1Factory *factory::d2dfactory = nullptr;
	__declspec(selectany) IDWriteFactory *factory::dwfactory = nullptr;
	__declspec(selectany) IWICImagingFactory *factory::wicfactory = nullptr;

	inline bool initialize()
	{
		try{
			_factory = new factory(factory::Init());
		}catch(std::bad_alloc&){
			return false;
		}
		return true;
	}

	inline void uninitialize()
	{
		if(_factory != nullptr){
			delete _factory;
			_factory = nullptr;
		}
	}

} }
