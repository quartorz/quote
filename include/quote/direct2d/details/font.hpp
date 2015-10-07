#include "../font.hpp"
#include "../utils.hpp"

namespace quote{ namespace direct2d{

	inline font::font(const font &f):
		format(nullptr),
		locale(f.locale),
		name(f.name),
		size(f.size),
		weight(f.weight),
		style(f.style),
		modified(true)
	{
	}

	inline font::~font()
	{
		::quote::utils::SafeRelease(format);
	}

	inline void font::set_name(const wchar_t *f)
	{
		modified = true;
		name = f;
	}

	inline void font::set_size(float s)
	{
		modified = true;
		size = s;
	}

	inline void font::set_weight(font_weight w)
	{
		weight = w;
	}

	inline void font::set_style(font_style s)
	{
		style = s;
	}

	inline bool font::create_resource(const creation_params &)
	{
		return create();
	}

	inline void font::destroy_resource()
	{
		modified = true;
		::quote::utils::SafeRelease(format);
	}

	inline bool font::create()
	{
		if(!modified)
			return true;

		if(SUCCEEDED(factory[DWrite]->CreateTextFormat(
			name.c_str(),
			NULL,
			static_cast<DWRITE_FONT_WEIGHT>(weight),
			static_cast<DWRITE_FONT_STYLE>(style),
			DWRITE_FONT_STRETCH_NORMAL,
			size,
			locale.c_str(),
			&format)))
		{
			modified = false;
			return true;
		}

		return false;
	}

	inline IDWriteTextFormat *font::Get()
	{
		if(modified){
			::quote::utils::SafeRelease(format);
			create();
		}

		return format;
	}

	inline IDWriteTextFormat *font::Get() const
	{
		return const_cast<font*>(this)->Get();
	}

} }
