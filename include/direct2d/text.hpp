#pragma once

#include <dwrite.h>

#include "font.hpp"
#include "object.hpp"
#include "solid_brush.hpp"

namespace quote{ namespace direct2d{

	class text: public object{
	public:
		enum class alignment: unsigned{
			left = DWRITE_TEXT_ALIGNMENT_LEADING,
			center = DWRITE_TEXT_ALIGNMENT_CENTER,
			right = DWRITE_TEXT_ALIGNMENT_TRAILING,
		};
		enum class paragraph_align: unsigned{
			top = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
			right = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		};
		enum class word_wrapping: unsigned{
			wrap = DWRITE_WORD_WRAPPING_WRAP,
			no_wrap = DWRITE_WORD_WRAPPING_NO_WRAP,
		};

		using font_weight = font::font_weight;

	private:
		factory factory_;
		IDWriteTextLayout *layout;
		solid_brush brush;
		font font_;
		std::wstring text_;
		word_wrapping wordwrapping;
		alignment align;
		paragraph_align p_align;
		std::vector<std::pair<DWRITE_TEXT_RANGE, float>> fontsizes;
		std::vector<std::pair<DWRITE_TEXT_RANGE, bool>> underlines, italics, strikethroughs;
		std::vector<std::pair<DWRITE_TEXT_RANGE, font_weight>> fontweights;
		std::vector<std::pair<DWRITE_TEXT_RANGE, solid_brush>> colors;
		bool modified;

		bool create();
		void set_colors();

	public:
		text():
			layout(nullptr),
			brush(color()),
			wordwrapping(word_wrapping::wrap),
			align(alignment::left),
			p_align(paragraph_align::top),
			modified(true)
		{
		}
		text(const text &);
		text(text &&);
		~text();
		
		void set_font(const font &);
		void set_font(font &&);
		void set_text(const wchar_t *);
		void set_color(color, int start=-1, int end=-1);
		void set_word_wrapping(word_wrapping);
		void set_alignment(alignment);
		void set_paragraph_align(paragraph_align);
		void set_underline(int start, int end, bool set=true);
		void set_italic(int start, int end, bool set=true);
		void set_strike_through(int start, int end, bool set=true);

		font &get_font();
		const font &get_font() const;
		const wchar_t *get_text() const;
		alignment get_alignment() const;
		paragraph_align get_paragraph_align() const;
		word_wrapping get_word_wrapping() const;

		virtual rect get_drawing_rect() override;
		virtual void set_size(const size &) override;
		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &) override;

		IDWriteTextLayout *Get();
	};

} }

#include "details/text.hpp"
