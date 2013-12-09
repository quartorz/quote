#pragma once

#include <dwrite.h>

#include "resource.hpp"
#include "factory.hpp"

#include <string>

namespace quote{ namespace direct2d{

	class font: public resource{
	public:
		enum class FontWeight: unsigned{
			Light = DWRITE_FONT_WEIGHT_LIGHT,
			Normal = DWRITE_FONT_WEIGHT_NORMAL,
			Bold = DWRITE_FONT_WEIGHT_BOLD,
		};

		enum class FontStyle: unsigned{
			Normal,
			Italic,
		};

	private:
		factory factory;
		IDWriteTextFormat *format;
		std::wstring locale, name;
		float size;
		FontWeight weight;
		FontStyle style;
		bool modified;

	public:
		font():
			format(nullptr),
			locale(L"ja-jp"),
			name(L"Meiryo"),
			size(18.f),
			weight(FontWeight::Normal),
			style(FontStyle::Normal),
			modified(true)
		{
		}
		font(const font &);
		~font();

		void set_locale(const wchar_t *);
		void set_name(const wchar_t *);
		void set_size(float);
		void set_weight(FontWeight);
		void set_style(FontStyle);

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;

		bool create();
		IDWriteTextFormat *Get();
		IDWriteTextFormat *Get() const;
	};

} }

#include "details/font.hpp"
