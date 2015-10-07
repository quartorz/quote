#pragma once

#include "../base_types.hpp"
#include "../brush.hpp"
#include "../font.hpp"

namespace quote{ namespace direct2d{

	inline void paint_params::draw_line(const brush &brush, const line &line, float width /* =1.f */) const
	{
		target->DrawLine(
			line.get_start_point(),
			line.get_end_point(),
			brush.Get(),
			width);
	}

	inline void paint_params::draw_rect(const brush &brush, const rect &rect, float width /* =1.f */) const
	{
		target->DrawRectangle(
			rect,
			brush.Get(),
			width);
	}

	inline void paint_params::fill_rect(const brush &brush, const rect &rect) const
	{
		target->FillRectangle(
			rect,
			brush.Get());
	}

	inline void paint_params::draw_circle(const brush &brush, const circle &circle, float width /* =1.f */) const
	{
		target->DrawEllipse(
			D2D1::Ellipse(circle.get_center(), circle.get_radius(), circle.get_radius()),
			brush.Get(),
			width);
	}

	inline void paint_params::fill_circle(const brush &brush, const circle &circle) const
	{
		target->FillEllipse(
			D2D1::Ellipse(circle.get_center(), circle.get_radius(), circle.get_radius()),
			brush.Get());
	}

	inline void paint_params::draw_text(const font &font, const brush &brush, const wchar_t *text, const rect &rect, bool clip /* =false */) const
	{
		target->DrawTextW(
			text,
			::wcslen(text),
			font.Get(),
			rect,
			brush.Get(),
			(clip? D2D1_DRAW_TEXT_OPTIONS_CLIP: D2D1_DRAW_TEXT_OPTIONS_NONE));
	}

	inline void paint_params::clear(const color &c) const
	{
		target->Clear(c);
	}

	inline void rect::draw(const paint_params &pp, const brush &brush, float width /* =1.f */)
	{
		pp.target->DrawRectangle(
			*this,
			brush.Get(),
			width);
	}

	inline void rect::fill(const paint_params &pp, const brush &brush)
	{
		pp.target->FillRectangle(
			*this,
			brush.Get());
	}

	inline void line::draw(const paint_params &pp, const brush &brush, float width /* =1.f */)
	{
		pp.target->DrawLine(
			p1,
			p2,
			brush.Get(),
			width);
	}

	inline void circle::draw(const paint_params &pp, const brush &brush, float width /* =1.f */)
	{
		pp.target->DrawEllipse(
			D2D1::Ellipse(center, radius, radius),
			brush.Get(),
			width);
	}

	inline void circle::fill(const paint_params &pp, const brush &brush)
	{
		pp.target->FillEllipse(
			D2D1::Ellipse(center, radius, radius),
			brush.Get());
	}

	inline void polygon::draw(const paint_params &pp , const brush &brush, float width /* =1.f */)
	{
		point p = get_position();

		D2D1::Matrix3x2F transform;

		pp.target->GetTransform(&transform);
		pp.target->SetTransform(transform * D2D1::Matrix3x2F::Translation(p.x, p.y));

		pp.target->DrawGeometry(
			geometry,
			brush.Get(),
			width);

		pp.target->SetTransform(transform);
	}

	inline void polygon::fill(const paint_params &pp, const brush &brush)
	{
		point p = get_position();

		D2D1::Matrix3x2F transform;

		pp.target->GetTransform(&transform);
		pp.target->SetTransform(transform * D2D1::Matrix3x2F::Translation(p.x, p.y));

		pp.target->FillGeometry(
			geometry,
			brush.Get());

		pp.target->SetTransform(transform);
	}

} }
