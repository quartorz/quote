#pragma once

#include "../image.hpp"
#include "../utils.hpp"

namespace quote{ namespace direct2d{

	inline bool image::create(ID2D1RenderTarget *t)
	{
		if(!modified)
			return true;

		modified = false;

		::quote::utils::SafeRelease(bmp);

		if(FAILED(decoder.decode(
			filename.c_str(),
			t,
			&bmp)))
			return false;

		calc_rectangle();

		return true;
	}

	inline void image::calc_rectangle()
	{
		if(clippingrect.width() <= 0.f || clippingrect.height() <= 0.f){
			auto s = bmp->GetSize();
			src = rect(0.f, 0.f, s.width, s.height);
		}else
			src = clippingrect;

		if(size_.width == 0.f || size_.height == 0.f){
			auto s = bmp->GetSize();
			dest = rect(pos, size(s.width, s.height));
		}else
			dest = rect(pos, size_);

		switch(drawingmode){
		case drawing_mode::stretch:
			break;
		case drawing_mode::squeeze:
			if(size_.width / size_.height < src.width() / src.height()){
				float x = src.height() * size_.width / src.width();
				dest.top = (dest.bottom - x) / 2;
				dest.bottom = dest.top + x;
			}else{
				float x = src.width() * size_.height / src.height();
				dest.left = (dest.right - x) / 2;
				dest.right = dest.left + x;
			}
			break;
		case drawing_mode::trim:
			if(src.width() / src.height() < size_.width / size_.height){
				float x = size_.height * src.width() / size_.width;
				src.top = (src.bottom - x) / 2;
				src.bottom = src.top + x;
			}else{
				float x = size_.width * src.height() / size_.height;
				src.left = (src.right - x) / 2;
				src.right = src.left + x;
			}
			break;
		default:
			__assume(0);
		}
	}

	inline image::image(const wchar_t *filename /* =L"" */):
		bmp(nullptr),
		filename(filename),
		drawingmode(drawing_mode::stretch),
		interpolationmode(interpolation_mode::linear),
		modified(true)
	{
	}

	inline image::image(const image &i):
		bmp(nullptr),
		filename(i.filename),
		drawingmode(i.drawingmode),
		interpolationmode(i.interpolationmode),
		modified(true)
	{
	}

	inline image::~image()
	{
		::quote::utils::SafeRelease(bmp);
	}

	inline void image::set_drawing_mode(drawing_mode mode)
	{
		drawingmode = mode;

		if(bmp != nullptr)
			calc_rectangle();
	}

	inline void image::set_interpolation_mode(interpolation_mode im)
	{
		interpolationmode = im;
	}

	inline void image::set_file_name(const wchar_t *f)
	{
		if(filename != f){
			modified = true;
			filename = f;
		}
	}

	inline void image::set_clipping_rect(rect r)
	{
		clippingrect = r;

		if(bmp != nullptr)
			calc_rectangle();
	}

	inline void image::set_position(const point &p)
	{
		object::set_position(p);

		if(bmp != nullptr)
			calc_rectangle();
	}

	inline void image::set_size(const size &s)
	{
		object::set_size(s);

		if(bmp != nullptr)
			calc_rectangle();
	}

	inline bool image::create_resource(const creation_params &cp)
	{
		return create(cp.target);
	}

	inline void image::destroy_resource()
	{
		::quote::utils::SafeRelease(bmp);
	}

	inline void image::draw(const paint_params &pp)
	{
		if(modified){
			destroy_resource();
			create(pp.target);
		}

		pp.target->DrawBitmap(
			bmp,
			dest,
			1.f,
			static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
			src);
	}

	inline ID2D1Bitmap *image::Get() const
	{
		return bmp;
	}

} }
