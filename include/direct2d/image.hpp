#pragma once

#include "base_types.hpp"
#include "object.hpp"
#include "wicdecoder.hpp"

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
		wicdecoder decoder;
		ID2D1Bitmap *bmp;
		std::wstring filename;
		rect clippingrect, src, dest;
		drawing_mode drawingmode;
		interpolation_mode interpolationmode;
		bool modified;

		bool create(ID2D1RenderTarget *);
		void calc_rectangle();

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
