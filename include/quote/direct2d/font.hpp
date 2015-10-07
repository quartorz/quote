#pragma once

#include <dwrite.h>

#include "resource.hpp"
#include "factory.hpp"

#include <string>

namespace quote{ namespace direct2d{

	// Cython‚Åenum class‚ªŽg‚¦‚È‚¢
	namespace font_weight{
		enum font_weight{
			light = DWRITE_FONT_WEIGHT_THIN,
			normal = DWRITE_FONT_WEIGHT_LIGHT,
			bold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
		};
	}

	namespace font_style{
		enum font_style{
			normal,
			italic,
		};
	}

	class font: public resource{
	public:
		enum class font_weight: unsigned{
			light = DWRITE_FONT_WEIGHT_THIN,
			normal = DWRITE_FONT_WEIGHT_NORMAL,
			bold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
		};

		enum class font_style: unsigned{
			normal,
			italic,
		};

	private:
		factory factory;
		IDWriteTextFormat *format;
		std::wstring locale, name;
		float size;
		font_weight weight;
		font_style style;
		bool modified;

	public:
		font():
			format(nullptr),
			locale(L"ja-jp"),
			name(L"Meiryo"),
			size(18.f),
			weight(font_weight::normal),
			style(font_style::normal),
			modified(true)
		{
		}
		font(const font &);
		~font();

		void set_locale(const wchar_t *);
		void set_name(const wchar_t *);
		void set_size(float);
		void set_weight(font_weight);
		void set_style(font_style);

		void set_weight(::quote::direct2d::font_weight::font_weight w)
		{
			set_weight(static_cast<font_weight>(w));
		}

		void set_style(::quote::direct2d::font_style::font_style s)
		{
			set_style(static_cast<font_style>(s));
		}

		float get_size();

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;

		bool create();
		IDWriteTextFormat *Get();
		IDWriteTextFormat *Get() const;
	};

} }

#include "impl/font.hpp"
