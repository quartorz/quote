#pragma once

#include "../brush.hpp"

namespace quote{ namespace direct2d{

	inline void rect::draw(const paint_params &ps, const brush &brush, float width /* =1.f */)
	{
		ps.target->DrawRectangle(
			*this,
			brush.Get(),
			width);
	}

	inline void rect::fill(const paint_params &ps, const brush &brush)
	{
		ps.target->FillRectangle(
			*this,
			brush.Get());
	}

	inline void line::draw(const paint_params &ps, const brush &brush, float width /* =1.f */)
	{
		ps.target->DrawLine(
			p1,
			p2,
			brush.Get(),
			width);
	}

	inline void circle::draw(const paint_params &ps, const brush &brush, float width /* =1.f */)
	{
		ps.target->DrawEllipse(
			D2D1::Ellipse(center, radius, radius),
			brush.Get(),
			width);
	}

	inline void circle::fill(const paint_params &ps, const brush &brush)
	{
		ps.target->FillEllipse(
			D2D1::Ellipse(center, radius, radius),
			brush.Get());
	}

	inline void polygon::draw(const paint_params &ps, const brush &brush, float width /* =1.f */)
	{
		point p = get_position();

		D2D1::Matrix3x2F transform;

		ps.target->GetTransform(&transform);
		ps.target->SetTransform(transform * D2D1::Matrix3x2F::Translation(p.x, p.y));

		ps.target->DrawGeometry(
			geometry,
			brush.Get(),
			width);

		ps.target->SetTransform(transform);
	}

	inline void polygon::fill(const paint_params &ps, const brush &brush)
	{
		point p = get_position();

		D2D1::Matrix3x2F transform;

		ps.target->GetTransform(&transform);
		ps.target->SetTransform(transform * D2D1::Matrix3x2F::Translation(p.x, p.y));

		ps.target->FillGeometry(
			geometry,
			brush.Get());

		ps.target->SetTransform(transform);
	}

} }
