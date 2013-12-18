#pragma once

#include "../text.hpp"
#include "../utils.hpp"

#include <tuple>
#include <algorithm>

namespace quote{ namespace direct2d{

	inline bool text::create()
	{
		if(!modified)
			return true;

		modified = false;

		font_.destroy_resource();
		::quote::utils::SafeRelease(layout);

		if(!font_.create())
			return false;

		if(FAILED(factory_[DWrite]->CreateTextLayout(
			text_.c_str(),
			text_.length(),
			font_.Get(),
			size_.width,
			size_.height,
			&layout)))
			return false;

		layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(align));
		layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(p_align));
		layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(wordwrapping));

		for(auto &p: fontsizes){
			layout->SetFontSize(std::get<1>(p), std::get<0>(p));
		}
		for(auto &p: underlines){
			layout->SetUnderline(std::get<1>(p), std::get<0>(p));
		}
		for(auto &p: italics){
			layout->SetFontStyle((std::get<1>(p)? DWRITE_FONT_STYLE_ITALIC: DWRITE_FONT_STYLE_NORMAL), std::get<0>(p));
		}
		for(auto &p: strikethroughs){
			layout->SetStrikethrough(std::get<1>(p), std::get<0>(p));
		}
		for(auto &p: fontweights){
			layout->SetFontWeight(static_cast<DWRITE_FONT_WEIGHT>(std::get<1>(p)), std::get<0>(p));
		}

		set_colors();

		return true;
	}

	inline void text::set_colors()
	{
		for(auto &p: colors){
			layout->SetDrawingEffect(std::get<1>(p).Get(), std::get<0>(p));
		}
	}

	inline text::text(const text &t):
		layout(nullptr),
		brush(t.brush),
		wordwrapping(t.wordwrapping),
		align(t.align),
		p_align(t.p_align),
		fontsizes(t.fontsizes),
		underlines(t.underlines),
		italics(t.italics),
		strikethroughs(t.strikethroughs),
		colors(t.colors),
		modified(true)
	{
	}

	inline text::text(text &&t):
		layout(nullptr),
		brush(t.brush),
		wordwrapping(t.wordwrapping),
		align(t.align),
		p_align(t.p_align),
		fontsizes(std::move(t.fontsizes)),
		underlines(std::move(t.underlines)),
		italics(std::move(t.italics)),
		strikethroughs(std::move(t.strikethroughs)),
		colors(t.colors),
		modified(true)
	{
	}

	inline text::~text()
	{
		destroy_resource();
	}

	inline void text::set_font(const font &f)
	{
		font_ = f;
	}

	inline void text::set_font(font &&f)
	{
		font_ = std::move(f);
	}

	inline void text::set_text(const wchar_t *t)
	{
		modified = true;
		text_ = t;
	}

	inline void text::set_color(color c, int start /* =-1 */, int end /* =-1 */)
	{
		if(start < 0 || end <= 0)
			brush.set_color(c);
		else
			colors.push_back(std::make_pair(::quote::utils::CreateRange(start, end), c));
	}

	inline void text::set_word_wrapping(word_wrapping ww)
	{
		wordwrapping = ww;

		if(layout != nullptr)
			layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(ww));
	}

	inline void text::set_alignment(alignment a)
	{
		align = a;

		if(layout != nullptr)
			layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(a));
	}

	inline void text::set_paragraph_align(paragraph_align pa)
	{
		p_align = pa;

		if(layout != nullptr)
			layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(pa));
	}

	inline void text::set_underline(int start, int end, bool set /* =true */)
	{
		if(start < 0 || end <= 0){
			underlines.clear();
			if(set)
				underlines.push_back(std::make_pair(::quote::utils::CreateRange(0, -1), true));
		} else
			underlines.push_back(std::make_pair(::quote::utils::CreateRange(start, end), set));
	}

	inline void text::set_italic(int start, int end, bool set /* =true */)
	{
		if(start < 0 || end <= 0){
			italics.clear();
			if(set)
				italics.push_back(std::make_pair(::quote::utils::CreateRange(0, -1), true));
		} else
			italics.push_back(std::make_pair(::quote::utils::CreateRange(start, end), set));
	}

	inline void text::set_strike_through(int start, int end, bool set /* =true */)
	{
		if(start < 0 || end <= 0){
			italics.clear();
			if(set)
				strikethroughs.push_back(std::make_pair(::quote::utils::CreateRange(0, -1), true));
		} else
			strikethroughs.push_back(std::make_pair(::quote::utils::CreateRange(start, end), set));
	}

	inline font &text::get_font()
	{
		return font_;
	}

	inline const font &text::get_font() const
	{
		return font_;
	}

	inline const wchar_t *text::get_text() const
	{
		return text_.c_str();
	}

	inline text::alignment text::get_alignment() const
	{
		return align;
	}

	inline text::paragraph_align text::get_paragraph_align() const
	{
		return p_align;
	}

	inline text::word_wrapping text::get_word_wrapping() const
	{
		return wordwrapping;
	}

	inline rect text::get_drawing_rect()
	{
		if(modified){
			::quote::utils::SafeRelease(layout);
			if(!create())
				throw std::runtime_error("could not create IDWriteTextLayout");
		}

		DWRITE_OVERHANG_METRICS dom;
		layout->GetOverhangMetrics(&dom);
		return rect(
			point(pos.x - dom.left, pos.y - dom.top),
			size(size_.width + dom.left + dom.right, size_.height + dom.top + dom.bottom));
	}

	inline bool text::create_resource(const creation_params &cs)
	{
		if(!brush.create_resource(cs))
			return false;
		if(!std::all_of(
			colors.begin(), colors.end(),
			[&cs](std::pair<DWRITE_TEXT_RANGE, solid_brush> &p){
			return p.second.create_resource(cs);
		}))
			return false;

		if(layout == nullptr){
			if(!create())
				return false;
		} else
			set_colors();

		return true;
	}

	inline void text::destroy_resource()
	{
		::quote::utils::SafeRelease(layout);
		brush.destroy_resource();
		std::for_each(
			colors.begin(), colors.end(),
			[](std::pair<DWRITE_TEXT_RANGE, solid_brush> &p){
			p.second.destroy_resource();
		});
	}

	inline void text::draw(const paint_params &ps)
	{
		if(modified){
			::quote::utils::SafeRelease(layout);
			create();
		}

		ps.target->DrawTextLayout(
			pos,
			layout,
			brush.Get());
	}

	inline IDWriteTextLayout *text::Get()
	{
		if(modified){
			::quote::utils::SafeRelease(layout);
			create();
		}

		return layout;
	}

} }
