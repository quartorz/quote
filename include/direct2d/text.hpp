#pragma once

#include <dwrite.h>

#include "font.hpp"
#include "object.hpp"
#include "solid_brush.hpp"

namespace quote{ namespace direct2d{

	class text: public object{
	public:
		enum class Align: unsigned{
			Left = DWRITE_TEXT_ALIGNMENT_LEADING,
			Center = DWRITE_TEXT_ALIGNMENT_CENTER,
			Right = DWRITE_TEXT_ALIGNMENT_TRAILING,
		};
		enum class ParagraphAlign: unsigned{
			Top = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
			Right = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		};
		enum class WordWrapping: unsigned{
			Wrap = DWRITE_WORD_WRAPPING_WRAP,
			NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP,
		};

		using FontWeight = font::FontWeight;

	private:
		factory factory_;
		IDWriteTextLayout *layout;
		solid_brush brush;
		font font_;
		std::wstring text_;
		WordWrapping wordwrapping;
		Align align;
		ParagraphAlign p_align;
		std::vector<std::pair<DWRITE_TEXT_RANGE, float>> fontsizes;
		std::vector<std::pair<DWRITE_TEXT_RANGE, bool>> underlines, italics, strikethroughs;
		std::vector<std::pair<DWRITE_TEXT_RANGE, FontWeight>> fontweights;
		std::vector<std::pair<DWRITE_TEXT_RANGE, solid_brush>> colors;
		bool modified;

		bool create();
		void set_colors();

	public:
		text():
			layout(nullptr),
			brush(color()),
			wordwrapping(WordWrapping::Wrap),
			align(Align::Left),
			p_align(ParagraphAlign::Top),
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
		void set_word_wrapping(WordWrapping);
		void set_align(Align);
		void set_paragraph_align(ParagraphAlign);
		void set_underline(int start, int end, bool set=true);
		void set_italic(int start, int end, bool set=true);
		void set_strike_through(int start, int end, bool set=true);

		font &get_font();
		const font &get_font() const;
		const wchar_t *get_text() const;
		Align get_align() const;
		ParagraphAlign get_paragraph_align() const;
		WordWrapping get_word_wrapping() const;

		virtual rect get_drawing_rect() override;
		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &) override;

		IDWriteTextLayout *Get();
	};

} }

#include "details/text.hpp"
