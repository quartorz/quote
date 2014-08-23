#pragma once

#include "../image.hpp"
#include "../utils.hpp"

#include "../../tmp/on_exit.hpp"

#include <wincodec.h>
#include <comdef.h>

#include <chrono>

namespace quote{ namespace direct2d{

	inline bool image::create(ID2D1RenderTarget *t)
	{
		if(is_gif)
			return create_gif(t);

		if(!modified)
			return true;

		is_animated = false;
		modified = false;

		::quote::utils::SafeRelease(bmp);

		try{
			_COM_SMARTPTR_TYPEDEF(IWICBitmapDecoder, __uuidof(IWICBitmapDecoder));
			_COM_SMARTPTR_TYPEDEF(IWICBitmapFrameDecode, __uuidof(IWICBitmapFrameDecode));
			_COM_SMARTPTR_TYPEDEF(IWICFormatConverter, __uuidof(IWICFormatConverter));

			IWICBitmapDecoderPtr decoder;
			::_com_util::CheckError(factory_[WIC]->CreateDecoderFromFilename(
				filename.c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&decoder));

			IWICBitmapFrameDecodePtr frame;
			::_com_util::CheckError(decoder->GetFrame(0, &frame));

			IWICFormatConverterPtr conv;
			::_com_util::CheckError(factory_[WIC]->CreateFormatConverter(&conv));

			::_com_util::CheckError(conv->Initialize(
				frame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut));

			::_com_util::CheckError(t->CreateBitmapFromWicBitmap(conv, &bmp));
		}catch(::_com_error &){
			return false;
		}

		calc_rectangle();

		return true;
	}

	inline bool image::create_gif(ID2D1RenderTarget *t)
	{
		if(!modified)
			return true;

		modified = false;

		::quote::utils::SafeRelease(bmp);
		utils::SafeRelease(composed);

		try{
			_COM_SMARTPTR_TYPEDEF(IWICBitmapFrameDecode, __uuidof(IWICBitmapFrameDecode));
			_COM_SMARTPTR_TYPEDEF(IWICFormatConverter, __uuidof(IWICFormatConverter));
			_COM_SMARTPTR_TYPEDEF(IWICMetadataQueryReader, __uuidof(IWICMetadataQueryReader));

			::_com_util::CheckError(factory_[WIC]->CreateDecoderFromFilename(
				filename.c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&decoder));

			IWICMetadataQueryReaderPtr reader;
			::_com_util::CheckError(decoder->GetMetadataQueryReader(&reader));

			background = get_bg_color(reader);

			PROPVARIANT var;
			::PropVariantInit(&var);

			is_animated = true;

			::_com_util::CheckError(::PropVariantClear(&var));

			::_com_util::CheckError(decoder->GetFrameCount(&framecount));
			if(framecount == 1)
				is_animated = false;

			// initialization of animated gif
			if(is_animated){
				::_com_util::CheckError(t->CreateCompatibleRenderTarget(
					&btarget));
			}
			disposal = 0;
			duration = std::chrono::milliseconds();
			timepoint = std::chrono::system_clock::now();
			select_frame(t, 0);

		}catch(::_com_error &){
			return false;
		}

		calc_rectangle();

		return true;
	}

	inline void image::calc_rectangle()
	{
		if(clippingrect.width() <= 0.f || clippingrect.height() <= 0.f){
			if(is_gif){
				src = rect();

				_COM_SMARTPTR_TYPEDEF(IWICMetadataQueryReader, __uuidof(IWICMetadataQueryReader));

				IWICMetadataQueryReaderPtr reader;
				decoder->GetMetadataQueryReader(&reader);
				if(reader != nullptr){
					PROPVARIANT var;
					::PropVariantInit(&var);
					if(SUCCEEDED(reader->GetMetadataByName(L"/logscrdesc/Width", &var))){
						if(var.vt == VT_UI2){
							src.right = var.uiVal;
						}
						::PropVariantClear(&var);
					}
					if(SUCCEEDED(reader->GetMetadataByName(L"/logscrdesc/Height", &var))){
						if(var.vt == VT_UI2){
							src.bottom = var.uiVal;
						}
						::PropVariantClear(&var);
					}
				}
			}else{
				auto s = bmp->GetSize();
				src = rect(0.f, 0.f, s.width, s.height);
			}
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

	inline color image::get_bg_color(IWICMetadataQueryReader *reader)
	{
		PROPVARIANT var;
		::PropVariantInit(&var);

		try{
			_COM_SMARTPTR_TYPEDEF(IWICPalette, __uuidof(IWICPalette));

			QUOTE_ON_EXIT{
				::PropVariantClear(&var);
			};

			::_com_util::CheckError(reader->GetMetadataByName(
				L"/logscrdesc/GlobalColorTableFlag",
				&var));
			if(var.vt != VT_BOOL || !var.boolVal){
				return color();
			}
			::PropVariantClear(&var);

			::_com_util::CheckError(reader->GetMetadataByName(
				L"/logscrdesc/BackgroundColorIndex",
				&var));
			if(var.vt != VT_UI1)
				return color();

			BYTE index = var.bVal;

			::PropVariantClear(&var);

			IWICPalettePtr palette;
			::_com_util::CheckError(factory_[WIC]->CreatePalette(&palette));

			decoder->CopyPalette(palette);

			WICColor colors[256];
			UINT colors_count = 0;
			::_com_util::CheckError(palette->GetColors(
				_countof(colors),
				colors,
				&colors_count));

			if(index >= colors_count)
				return color();

			DWORD dwcolor = colors[index];

			return color(
				static_cast<int>(dwcolor >> 16) & 0xff,
				static_cast<int>(dwcolor >> 8) & 0xff,
				static_cast<int>(dwcolor) & 0xff,
				static_cast<int>(dwcolor >> 24));

			auto c = color(static_cast<D2D1::ColorF::Enum>(dwcolor & 0xffffff));
			c.a = static_cast<float>(dwcolor >> 24) / 255.f;
			return c;
		}catch(::_com_error &){
			return color();
		}
	}

	inline void image::save_composed_frame()
	{
		try{
			_COM_SMARTPTR_TYPEDEF(ID2D1Bitmap, __uuidof(ID2D1Bitmap));

			ID2D1BitmapPtr frame;

			::_com_util::CheckError(btarget->GetBitmap(&frame));

			if(composed == nullptr){
				D2D1_SIZE_U bitmapSize = frame->GetPixelSize();
				D2D1_BITMAP_PROPERTIES bitmapProp;
				frame->GetDpi(&bitmapProp.dpiX, &bitmapProp.dpiY);
				bitmapProp.pixelFormat = frame->GetPixelFormat();

				::_com_util::CheckError(btarget->CreateBitmap(
					bitmapSize,
					bitmapProp,
					&composed));
			}

			composed->CopyFromBitmap(nullptr, frame, nullptr);

		}catch(::_com_error &){
			return;
		}
	}

	inline void image::dispose_current_frame()
	{
		switch(disposal){
		case 0:		// undefined
		case 1:		// none
			break;
		case 2:		// fill with background color
			btarget->BeginDraw();
			btarget->PushAxisAlignedClip(framepos, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			btarget->Clear(background);
			btarget->PopAxisAlignedClip();
			btarget->EndDraw();
			break;
		case 3:		// restore previous frame
			if(composed != nullptr){
				_COM_SMARTPTR_TYPEDEF(ID2D1Bitmap, __uuidof(ID2D1Bitmap));
				ID2D1BitmapPtr frame;
				btarget->GetBitmap(&frame);
				frame->CopyFromBitmap(nullptr, composed, nullptr);
			}
			break;
		}
	}

	inline void image::select_frame(ID2D1RenderTarget *t, UINT framecount)
	{
		_COM_SMARTPTR_TYPEDEF(ID2D1Bitmap, __uuidof(ID2D1Bitmap));
		ID2D1BitmapPtr bitmap;

		dispose_current_frame();

		try{
			_COM_SMARTPTR_TYPEDEF(IWICBitmapDecoder, __uuidof(IWICBitmapDecoder));
			_COM_SMARTPTR_TYPEDEF(IWICBitmapFrameDecode, __uuidof(IWICBitmapFrameDecode));
			_COM_SMARTPTR_TYPEDEF(IWICFormatConverter, __uuidof(IWICFormatConverter));
			_COM_SMARTPTR_TYPEDEF(IWICMetadataQueryReader, __uuidof(IWICMetadataQueryReader));

			IWICBitmapFrameDecodePtr frame;
			::_com_util::CheckError(decoder->GetFrame(framecount, &frame));

			IWICFormatConverterPtr conv;
			::_com_util::CheckError(factory_[WIC]->CreateFormatConverter(&conv));

			::_com_util::CheckError(conv->Initialize(
				frame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut));

			::_com_util::CheckError(t->CreateBitmapFromWicBitmap(conv, &bitmap));

			IWICMetadataQueryReaderPtr reader;
			::_com_util::CheckError(frame->GetMetadataQueryReader(&reader));

			PROPVARIANT var;
			::PropVariantInit(&var);

			QUOTE_ON_EXIT{
				::PropVariantClear(&var);
			};

			if(SUCCEEDED(reader->GetMetadataByName(L"/imgdesc/Left", &var))){
				if(var.vt == VT_UI2){
					framepos.left = static_cast<FLOAT>(var.uiVal);
				}
				::PropVariantClear(&var);
			}
			if(SUCCEEDED(reader->GetMetadataByName(L"/imgdesc/Top", &var))){
				if(var.vt == VT_UI2){
					framepos.top = static_cast<FLOAT>(var.uiVal);
				}
				::PropVariantClear(&var);
			}
			if(SUCCEEDED(reader->GetMetadataByName(L"/imgdesc/Width", &var))){
				if(var.vt == VT_UI2){
					framepos.right = static_cast<FLOAT>(var.uiVal) + framepos.left;
				}
				::PropVariantClear(&var);
			}
			if(SUCCEEDED(reader->GetMetadataByName(L"/imgdesc/Height", &var))){
				if(var.vt == VT_UI2){
					framepos.bottom = static_cast<FLOAT>(var.uiVal) + framepos.top;
				}
				::PropVariantClear(&var);
			}

			if(SUCCEEDED(reader->GetMetadataByName(L"/grctlext/Delay", &var)) && var.vt == VT_UI2){
				if(var.uiVal > 1)
					delay = std::chrono::duration<long long, std::centi>(var.uiVal);
				else
					delay = std::chrono::duration<long long, std::centi>(9);
			} else{
				delay = std::chrono::duration<long long, std::centi>(9);
			}

			if(SUCCEEDED(reader->GetMetadataByName(L"/grctlext/Disposal", &var))){
				if(var.vt == VT_UI1){
					disposal = var.bVal;
				}
			}else{
				disposal = 0;
			}

			::PropVariantClear(&var);

			if(disposal == 3){
				save_composed_frame();
			}

			btarget->BeginDraw();

			if(framecount == 0){
				btarget->Clear(background);
			}
			btarget->DrawBitmap(bitmap, framepos);

			btarget->EndDraw();

		}catch(::_com_error &){
			return;
		}

		currentframe = framecount;

		utils::SafeRelease(bmp);
		bmp = bitmap.Detach();
	}

	inline image::image(const wchar_t *filename /* =L"" */):
		bmp(nullptr),
		filename(filename),
		drawingmode(drawing_mode::stretch),
		interpolationmode(interpolation_mode::linear),
		modified(true),
		composed(nullptr)
	{
	}

	inline image::image(const image &i):
		bmp(nullptr),
		filename(i.filename),
		drawingmode(i.drawingmode),
		interpolationmode(i.interpolationmode),
		modified(true),
		composed(nullptr)
	{
	}

	inline image::~image()
	{
		destroy_resource();
		utils::SafeRelease(composed);
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

			auto pos = filename.find_last_of(L'.');
			is_gif = pos != std::wstring::npos && !::_wcsicmp(&filename[pos], L".gif");
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

		if(bmp == nullptr)
			return;

		if(is_animated){
			auto tp = std::chrono::system_clock::now();
			duration += std::chrono::duration_cast<std::chrono::milliseconds>(tp - timepoint);
			timepoint = tp;
			auto step = duration.count() / (delay.count() * 10);
			if(step != 0){
				duration = std::chrono::milliseconds(duration.count() % (delay.count() * 10));

				UINT i = 0;
				for(;;){
					pp.target->DrawBitmap(
						bmp,
						dest,
						1.f,
						static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
						src);

					i++;
					UINT f = (currentframe + i) % framecount;
					select_frame(pp.target, f);

					if(i == step)
						break;

					ID2D1Bitmap *frame;
					btarget->GetBitmap(&frame);
					if(frame != nullptr){
						pp.target->DrawBitmap(
							frame,
							dest,
							1.f,
							static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
							src);
						utils::SafeRelease(frame);
					}
				}
			}else
				pp.target->DrawBitmap(
					bmp,
					dest,
					1.f,
					static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
					src);
		}

		if(is_gif){
			ID2D1Bitmap *frame;
			btarget->GetBitmap(&frame);
			if(frame != nullptr){
				pp.target->DrawBitmap(
					frame,
					dest,
					1.f,
					static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
					src);
				utils::SafeRelease(frame);
			}
		}else{
			pp.target->DrawBitmap(
				bmp,
				dest,
				1.f,
				static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationmode),
				src);
		}
	}

	inline ID2D1Bitmap *image::Get() const
	{
		return bmp;
	}

} }
