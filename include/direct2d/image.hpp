#pragma once

#include "base_types.hpp"
#include "object.hpp"
#include "wicdecoder.hpp"
#include "factory.hpp"

#include <chrono>

namespace quote{ namespace direct2d{

	class image: public object{
	public:
		enum class drawing_mode: int{
			stretch,
			squeeze,
			trim,
		};
		enum class interpolation_mode: unsigned{
			nearest_neighbor = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			linear = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		};

	private:
		factory factory_;
		ID2D1Bitmap *bmp;
		std::wstring filename;
		rect clippingrect, src, dest;
		drawing_mode drawingmode;
		interpolation_mode interpolationmode;
		bool modified;

		bool is_gif, is_animated;

		// for animated gif
		_COM_SMARTPTR_TYPEDEF(ID2D1BitmapRenderTarget, __uuidof(ID2D1BitmapRenderTarget));
		_COM_SMARTPTR_TYPEDEF(IWICBitmapDecoder, __uuidof(IWICBitmapDecoder));
		std::chrono::duration<long long, std::centi> delay;
		std::chrono::milliseconds duration;
		std::chrono::system_clock::time_point timepoint;
		ID2D1BitmapRenderTargetPtr btarget;
		IWICBitmapDecoderPtr decoder;
		ID2D1Bitmap *composed;
		rect framepos;
		BYTE disposal;
		UINT framecount, currentframe;
		color background;

		bool create(ID2D1RenderTarget *);
		bool create_gif(ID2D1RenderTarget *);
		void calc_rectangle();

		//for animated gif
		color get_bg_color(IWICMetadataQueryReader *reader);
		void save_composed_frame();
		void dispose_current_frame();
		void select_frame(ID2D1RenderTarget *t, UINT frame);

	public:
		image(const wchar_t *filename=L"");
		image(const image &);
		~image();

		void set_drawing_mode(drawing_mode);
		void set_interpolation_mode(interpolation_mode);
		void set_file_name(const wchar_t *);
		void set_clipping_rect(rect);
		rect get_clipping_rect() const;

		virtual void set_position(const point &) override;
		virtual void set_size(const size &) override;

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &) override;

		ID2D1Bitmap *Get() const;
	};

} }

#include "details/image.hpp"
