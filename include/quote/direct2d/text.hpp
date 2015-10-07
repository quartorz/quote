#pragma once

#include <dwrite.h>

#include <tuple>
#include <functional>

#include "font.hpp"
#include "object.hpp"
#include "solid_brush.hpp"

#include "detail/CustomTextRenderer.h"

namespace quote{ namespace direct2d{

	// Cython‚Åenum class‚ªŽg‚¦‚È‚¢
	namespace text_alignment{
		enum text_alignment{
			left = DWRITE_TEXT_ALIGNMENT_LEADING,
			center = DWRITE_TEXT_ALIGNMENT_CENTER,
			right = DWRITE_TEXT_ALIGNMENT_TRAILING,
		};
	}

	namespace text_paragraph_align{
		enum text_paragraph_align{
			top = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
			bottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		};
	}

	namespace text_word_wrapping{
		enum text_word_wrapping{
			wrap = DWRITE_WORD_WRAPPING_WRAP,
			no_wrap = DWRITE_WORD_WRAPPING_NO_WRAP,
		};
	}

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
		using links_type = std::vector<std::pair<DWRITE_TEXT_RANGE, std::function<void(int, int)>>>;

		factory factory_;
		IDWriteTextLayout *layout;
		detail::CustomTextRenderer *renderer;
		solid_brush brush, link_brush;
		font font_;
		std::wstring text_;
		word_wrapping wordwrapping;
		alignment align;
		paragraph_align p_align;
		std::vector<std::pair<DWRITE_TEXT_RANGE, float>> fontsizes;
		std::vector<std::pair<DWRITE_TEXT_RANGE, bool>> underlines, italics, strikethroughs;
		std::vector<std::pair<DWRITE_TEXT_RANGE, font_weight>> fontweights;
		std::vector<std::pair<DWRITE_TEXT_RANGE, solid_brush>> colors;
		links_type links;
		bool modified;

		bool selectable;
		bool dragging, pressing, start_trailing;
		detail::TextSelection *selection;
		DWRITE_TEXT_RANGE select_range;
		UINT32 startpos, selectingpos;

		bool create();
		void apply_color();
		void apply_underline();

		std::tuple<UINT32, bool, bool> hittest_(const point &);
		links_type::iterator get_link(const point &);

	public:
		text():
			layout(nullptr),
			renderer(nullptr),
			brush(color()),
			wordwrapping(word_wrapping::wrap),
			align(alignment::left),
			p_align(paragraph_align::top),
			modified(true),
			selectable(false),
			dragging(false),
			pressing(false),
			start_trailing(false),
			select_range({0, 0}),
			selection(new detail::TextSelection)
		{
		}
		text(const text &);
		text(text &&);
		~text();
		
		void set_font(const font &);
		void set_font(font &&);
		void set_text(const wchar_t *);
		void set_color(color, int start = -1, int end = -1);
		void set_weight(font::font_weight weight, int start = 0, int end = -1);
		void set_link(std::function<void(int,int)> callback, int start, int end);
		void set_word_wrapping(word_wrapping);
		void set_alignment(alignment);
		void set_paragraph_align(paragraph_align);
		void set_underline(int start, int end, bool set=true);
		void set_italic(int start, int end, bool set=true);
		void set_strike_through(int start, int end, bool set=true);
		void set_selectable(bool selectable=true);
		void set_select_background_color(const color &);
		void set_select_text_color(const color &);
		void set_link_color(const color &);

		void set_weight(::quote::direct2d::font_weight::font_weight w, int start, int end)
		{
			set_weight(static_cast<font::font_weight>(w), start, end);
		}

		void set_word_wrapping(::quote::direct2d::text_word_wrapping::text_word_wrapping ww)
		{
			set_word_wrapping(static_cast<word_wrapping>(ww));
		}
		void set_alignment(::quote::direct2d::text_alignment::text_alignment a)
		{
			set_alignment(static_cast<alignment>(a));
		}
		void set_paragraph_align(::quote::direct2d::text_paragraph_align::text_paragraph_align pa)
		{
			set_paragraph_align(static_cast<paragraph_align>(pa));
		}

		font &get_font();
		const font &get_font() const;
		const wchar_t *get_text() const;
		alignment get_alignment() const;
		paragraph_align get_paragraph_align() const;
		word_wrapping get_word_wrapping() const;
		bool is_selectable();
		color get_select_background_color();
		color get_select_text_color();
		std::wstring get_selecting_text();
		std::tuple<int, int> get_selecting_range();

		::quote::direct2d::text_alignment::text_alignment get_alignment_() const
		{
			return static_cast<::quote::direct2d::text_alignment::text_alignment>(align);
		}
		::quote::direct2d::text_paragraph_align::text_paragraph_align get_paragraph_align_() const
		{
			return static_cast<::quote::direct2d::text_paragraph_align::text_paragraph_align>(p_align);
		}
		::quote::direct2d::text_word_wrapping::text_word_wrapping get_word_wrapping_() const
		{
			return static_cast<::quote::direct2d::text_word_wrapping::text_word_wrapping>(wordwrapping);
		}

		virtual bool is_colliding(const point &ap) override;
		virtual void on_left_press(const point &, hittest &) override;
		virtual void on_left_release(const point &, hittest &) override;
		virtual void on_mouse_move(const point &, hittest &) override;
		virtual void on_mouse_leave(const hittest &) override;
		virtual void on_lose_focus(const hittest &) override;

		virtual rect get_drawing_rect() override;
		virtual void set_size(const size &) override;
		virtual bool create_resource(const creation_params &) override;
		virtual void destroy_resource() override;
		virtual void draw(const paint_params &) override;

		IDWriteTextLayout *Get();
	};

} }

#include "impl/text.hpp"
