#pragma once

#include <dwrite.h>

#include "resource.hpp"
#include "factory.hpp"

#include <string>

namespace quote{ namespace direct2d{

	class font: public resource{
	public:
		enum class font_weight: unsigned{
			light = DWRITE_FONT_WEIGHT_LIGHT,
			normal = DWRITE_FONT_WEIGHT_NORMAL,
			bold = DWRITE_FONT_WEIGHT_BOLD,
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

		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;

		bool create();
		IDWriteTextFormat *Get();
		IDWriteTextFormat *Get() const;
	};

} }

#include "details/font.hpp"
