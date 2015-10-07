#pragma once

#include "../text.hpp"
#include "../utils.hpp"

#include <tuple>
#include <algorithm>
#include <exception>

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
			static_cast<UINT32>(text_.length()),
			font_.Get(),
			size_.width,
			size_.height,
			&layout)))
			return false;

		layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(align));
		layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(p_align));
		layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(wordwrapping));

		layout->SetFontWeight(DWRITE_FONT_WEIGHT_LIGHT, {0, static_cast<UINT32>(-1)});

		for(auto &p: fontsizes){
			layout->SetFontSize(std::get<1>(p), std::get<0>(p));
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

		apply_color();
		apply_underline();

		return true;
	}

	inline void text::apply_color()
	{
		for(auto &p: colors){
			layout->SetDrawingEffect(std::get<1>(p).Get(), std::get<0>(p));
		}

		for(auto &l: links){
			layout->SetDrawingEffect(link_brush.Get(), std::get<0>(l));
		}
	}

	inline void text::apply_underline()
	{
		for(auto &p : underlines){
			layout->SetUnderline(std::get<1>(p), std::get<0>(p));
		}
	}

	inline std::tuple<UINT32, bool, bool> text::hittest_(const point &p)
	{
		BOOL trailing, inside;
		DWRITE_HIT_TEST_METRICS metr;
		layout->HitTestPoint(
			p.x - pos.x,
			p.y - pos.y,
			&trailing,
			&inside,
			&metr);
		return std::make_tuple(metr.textPosition, trailing != 0, inside != 0);
	}

	inline text::links_type::iterator text::get_link(const point &pt)
	{
		UINT32 p;
		bool inside;

		std::tie(p, std::ignore, inside) = hittest_(pt);

		if(inside){
			for(auto iter = links.begin(); iter != links.end(); ++iter){
				auto &r = std::get<0>(*iter);

				if(r.startPosition <= p && p < r.startPosition + r.length){
					return iter;
				}
			}
		}

		return links.end();
	}

	inline text::text(const text &t):
		layout(nullptr),
		renderer(nullptr),
		brush(t.brush),
		link_brush(t.link_brush),
		wordwrapping(t.wordwrapping),
		align(t.align),
		p_align(t.p_align),
		fontsizes(t.fontsizes),
		underlines(t.underlines),
		italics(t.italics),
		strikethroughs(t.strikethroughs),
		fontweights(t.fontweights),
		colors(t.colors),
		modified(true),
		selectable(t.selectable),
		dragging(false),
		pressing(false),
		start_trailing(false),
		select_range({0, 0}),
		selection(new detail::TextSelection)
	{
		D2D1_COLOR_F c;
		t.selection->GetColor(&c);
		selection->SetColor(c);
		t.selection->GetTextColor(&c);
		selection->SetTextColor(c);
	}

	inline text::text(text &&t):
		layout(nullptr),
		renderer(nullptr),
		brush(t.brush),
		link_brush(t.link_brush),
		wordwrapping(t.wordwrapping),
		align(t.align),
		p_align(t.p_align),
		fontsizes(std::move(t.fontsizes)),
		underlines(std::move(t.underlines)),
		italics(std::move(t.italics)),
		strikethroughs(std::move(t.strikethroughs)),
		fontweights(std::move(t.fontweights)),
		colors(t.colors),
		modified(true),
		selectable(t.selectable),
		dragging(false),
		pressing(false),
		start_trailing(false),
		select_range({0, 0}),
		selection(t.selection)
	{
		::quote::utils::SafeAcquire(selection);
	}

	inline text::~text()
	{
		destroy_resource();
		::quote::utils::SafeRelease(selection);
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

	inline void text::set_weight(font::font_weight weight, int start /* =0 */, int end /* =-1 */)
	{
		fontweights.push_back(std::make_pair(::quote::utils::CreateRange(start, end), weight));

		if(layout != nullptr){
			layout->SetFontWeight(static_cast<DWRITE_FONT_WEIGHT>(weight), ::quote::utils::CreateRange(start, end));
		}
	}

	inline void text::set_link(std::function<void(int, int)> callback, int start, int end)
	{
		links.push_back({{static_cast<UINT32>(start), static_cast<UINT32>(end)}, callback});
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

	inline void text::set_selectable(bool s /* =true */)
	{
		selectable = s;
	}

	inline void text::set_select_background_color(const color &c)
	{
		selection->SetColor(c);
	}

	inline void text::set_select_text_color(const color &c)
	{
		selection->SetTextColor(c);
	}

	inline void text::set_link_color(const color &c)
	{
		link_brush.set_color(c);
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

	inline bool text::is_selectable()
	{
		return selectable;
	}

	inline color text::get_select_background_color()
	{
		color c;
		selection->GetColor(&c);
		return c;
	}
	inline color text::get_select_text_color()
	{
		color c;
		selection->GetTextColor(&c);
		return c;
	}

	inline std::wstring text::get_selecting_text()
	{
		if(!selectable)
			return L"";

		return text_.substr(select_range.startPosition, select_range.length);
	}

	inline std::tuple<int, int> text::get_selecting_range()
	{
		if(!selectable)
			return std::make_tuple(-1, -1);

		return std::make_tuple(select_range.startPosition, select_range.startPosition + select_range.length - 1);
	}

	inline rect text::get_drawing_rect()
	{
		if(modified){
			::quote::utils::SafeRelease(layout);
			if(!create())
				throw std::runtime_error("text::create failed in text::get_drawing_rect");
		}

		DWRITE_OVERHANG_METRICS dom;
		layout->GetOverhangMetrics(&dom);
		return rect(
			point(pos.x - dom.left, pos.y - dom.top),
			size(size_.width + dom.left + dom.right, size_.height + dom.top + dom.bottom));
	}

	inline void text::set_size(const size &s)
	{
		this->object::set_size(s);

		if(layout != nullptr){
			layout->SetMaxWidth(s.width);
			layout->SetMaxHeight(s.height);
		}
	}

	inline bool text::create_resource(const creation_params &cs)
	{
		if(!brush.create_resource(cs))
			return false;
		if(!link_brush.create_resource(cs))
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
			apply_color();

		renderer = new detail::CustomTextRenderer(factory_[D2D], cs.target);

		return true;
	}

	inline bool text::is_colliding(const point &ap)
	{
		if(layout == nullptr){
			if(!create())
				throw std::runtime_error("text::create failed in text::is_colliding");
		}

		return std::get<2>(hittest_(ap));
	}
	inline void text::on_left_press(const point &p, hittest &ht)
	{
		if(!selectable)
			return;

		dragging = false;
		pressing = true;
		selectingpos = -1;

		std::tie(startpos, start_trailing, std::ignore) = hittest_(p);

		if(links.size() != 0 && get_link(p) != links.end())
			ht.set_cursor(hittest::cursor::hand);
		else
			ht.set_cursor(hittest::cursor::ibeam);
	}

	inline void text::on_left_release(const point &pt, hittest &ht)
	{
		if(!dragging && links.size() != 0){
			auto iter = get_link(pt);

			if(iter != links.end()){
				auto &r = std::get<0>(*iter);
				ht.set_cursor(hittest::cursor::hand);
				std::get<1>(*iter)(r.startPosition, r.startPosition + r.length);
			}
		}

		if(!selectable){
			pressing = false;
			if(!dragging){
				if(select_range.length != -1){
					layout->SetDrawingEffect(nullptr, {0, static_cast<UINT32>(-1)});
					apply_color();
					::InvalidateRect(ht.hwnd, nullptr, FALSE);
				}
			}else{
				ht.set_cursor(hittest::cursor::ibeam);
			}

			dragging = false;
		}
	}

	inline void text::on_mouse_move(const point &pt, hittest &ht)
	{
		if(selectable){
			if(pressing){
				dragging = true;

				ht.set_cursor(hittest::cursor::ibeam);

				UINT32 p;
				bool trailing, inside;

				std::tie(p, trailing, inside) = hittest_(pt);

				layout->SetDrawingEffect(nullptr, {0, static_cast<UINT32>(-1)});
				apply_color();

				if(startpos != p || start_trailing != trailing){
					if(start_trailing){
						if(startpos < p){
							select_range.startPosition = startpos + 1;
							select_range.length = p - startpos;
						}else if(startpos > p){
							select_range.startPosition = p;
							select_range.length = startpos - p + 1;
						}else if(!trailing){
							select_range.startPosition = startpos;
							select_range.length = 1;
						}
					}else if(!start_trailing){
						if(startpos < p){
							select_range.startPosition = startpos;
							select_range.length = p - startpos + 1;
						}else if(startpos > p){
							select_range.startPosition = p;
							select_range.length = startpos - p;
						}else if(trailing){
							select_range.startPosition = startpos;
							select_range.length = 1;
						}
					}
					if(startpos < p && !trailing){
						select_range.length--;
					}else if(startpos > p && trailing){
						select_range.startPosition++;
						select_range.length--;
					}
					layout->SetDrawingEffect(selection, select_range);
					::InvalidateRect(ht.hwnd, nullptr, FALSE);
				}
			}else{
				if(std::get<2>(hittest_(pt))){
					ht.set_cursor(hittest::cursor::ibeam);
				}
			}
		}

		if((!selectable && !dragging) && links.size() != 0){
			auto iter = get_link(pt);

			if(iter != links.end()){
				ht.set_cursor(hittest::cursor::hand);
				layout->SetUnderline(TRUE, std::get<0>(*iter));
			}else{
				layout->SetUnderline(FALSE, {0, static_cast<UINT32>(-1)});
				apply_underline();
			}
			::InvalidateRect(ht.hwnd, nullptr, FALSE);
		}
	}

	inline void text::on_mouse_leave(const hittest &ht)
	{
		if(links.size() != 0){
			layout->SetUnderline(FALSE, {0, static_cast<UINT32>(-1)});
			apply_underline();
			::InvalidateRect(ht.hwnd, nullptr, FALSE);
		}
	}

	inline void text::on_lose_focus(const hittest &)
	{
		if(!selectable)
			return;

		layout->SetDrawingEffect(nullptr, {0, static_cast<UINT32>(-1)});
		apply_color();

		select_range = {0, 0};
	}

	inline void text::destroy_resource()
	{
		::quote::utils::SafeRelease(layout);
		::quote::utils::SafeRelease(renderer);
		brush.destroy_resource();
		link_brush.destroy_resource();
		std::for_each(
			colors.begin(), colors.end(),
			[](std::pair<DWRITE_TEXT_RANGE, solid_brush> &p){
			p.second.destroy_resource();
		});
		modified = true;
	}

	inline void text::draw(const paint_params &ps)
	{
		if(modified){
			::quote::utils::SafeRelease(layout);
			if(!create())
				throw std::runtime_error("text::create failed in text::draw");
		}

		layout->Draw(
			brush.Get(),
			renderer,
			pos.x,
			pos.y);
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
