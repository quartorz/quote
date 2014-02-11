#pragma once

#include <d2d1.h>
#include <cmath>
#include <Uxtheme.h>
#include <comdef.h>

#include "utils.hpp"
#include "factory.hpp"

#ifdef max
#undef max
#undef min
#endif

#include <vector>
#include <initializer_list>

namespace quote{ namespace direct2d{

	class brush;
	class font;
	struct line;
	struct circle;
	struct rect;
	struct color;

	struct creation_params{
		ID2D1RenderTarget *target;
	};

	struct paint_params{
		ID2D1RenderTarget *target;
		bool debug;

		void draw_line(const brush &brush, const line &line, float width=1.f) const;
		void draw_line(const brush *brush, const line &line, float width=1.f) const
		{
			draw_line(*brush, line, width);
		}
		void draw_rect(const brush &brush, const rect &rect, float width=1.f) const;
		void draw_rect(const brush *brush, const rect &rect, float width=1.f) const
		{
			draw_rect(*brush, rect, width);
		}
		void fill_rect(const brush &brush, const rect &rect) const;
		void fill_rect(const brush *brush, const rect &rect) const
		{
			fill_rect(*brush, rect);
		}
		void draw_circle(const brush &brush, const circle &circle, float width=1.f) const;
		void draw_circle(const brush *brush, const circle &circle, float width=1.f) const
		{
			draw_circle(*brush, circle, width);
		}
		void fill_circle(const brush &brush, const circle &circle) const;
		void fill_circle(const brush *brush, const circle &circle) const
		{
			fill_circle(*brush, circle);
		}
		void draw_text(const font &font, const brush &brush, const wchar_t *text, const rect &rect, bool clip=false) const;
		void draw_text(const font *font, const brush &brush, const wchar_t *text, const rect &rect, bool clip=false) const
		{
			draw_text(*font, brush, text, rect, clip);
		}
		void draw_text(const font &font, const brush *brush, const wchar_t *text, const rect &rect, bool clip=false) const
		{
			draw_text(font, *brush, text, rect, clip);
		}
		void draw_text(const font *font, const brush *brush, const wchar_t *text, const rect &rect, bool clip=false) const
		{
			draw_text(*font, *brush, text, rect, clip);
		}
		void clear(const color &) const;
	};

	struct size: public D2D1_SIZE_F{
		size(float w=0.f, float h=0.f):
			D2D1_SIZE_F(D2D1::SizeF(w, h))
		{
		}
		template <class T>
		size(const T &w, const T &h):
			D2D1_SIZE_F(D2D1::SizeF(static_cast<FLOAT>(w), static_cast<FLOAT>(h)))
		{
		}
		size &operator=(const D2D1_SIZE_U &s)
		{
			width = static_cast<FLOAT>(s.width);
			height = static_cast<FLOAT>(s.height);
			return *this;
		}
		size &operator=(const D2D1_SIZE_F &s)
		{
			width = s.width;
			height = s.height;
			return *this;
		}
	};
	inline size operator+(size s1, const size &s2)
	{
		s1.width += s2.width;
		s1.height += s2.height;
		return s1;
	}
	inline size operator-(size s1, const size &s2)
	{
		s1.width -= s2.width;
		s1.height -= s2.height;
		return s1;
	}
	struct rect;
	class polygon;
	struct point: public D2D1_POINT_2F{
		point(float x=0.f, float y=0.f):
			D2D1_POINT_2F(D2D1::Point2F(x, y))
		{
		}
		template <class T>
		point(const T &x, const T &y):
			D2D1_POINT_2F(D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y)))
		{
		}
		bool is_inside(const rect &) const;
		bool is_inside(const polygon &) const;
	};
	inline point operator+(point p1, const point &p2)
	{
		p1.x += p2.x;
		p1.y += p2.y;
		return p1;
	}
	inline point operator-(point p1, const point &p2)
	{
		p1.x -= p2.x;
		p1.y -= p2.y;
		return p1;
	}
	inline point operator*(point p, float f)
	{
		return point(p.x * f, p.y * f);
	}
	inline point operator*(float f, point p)
	{
		return p * f;
	}
	struct rect: public D2D1_RECT_F{
		rect(float left=0.f, float top=0.f, float right=0.f, float bottom=0.f):
			D2D1_RECT_F(D2D1::RectF(left, top, right, bottom))
		{
		}
		rect(const point &p, const size &s):
			D2D1_RECT_F(D2D1::RectF(p.x, p.y, p.x + s.width, p.y + s.height))
		{
		}
		float width() const
		{
			return right - left;
		}
		float height() const
		{
			return bottom - top;
		}
		bool is_colliding(const rect &r)
		{
			return left < r.right && right > r.left && top < r.bottom && bottom > r.top;
		}
		void draw(const paint_params &pp, const brush &brush, float width=1.f);
		void draw(const paint_params &pp, const brush *brush, float width=1.f)
		{
			draw(pp, *brush, width);
		}
		void fill(const paint_params &pp, const brush &brush);
		void fill(const paint_params &pp, const brush *brush)
		{
			fill(pp, *brush);
		}
	};

	struct line{
		point p1, p2;

		line(const point &p1=point(), const point &p2=point()): p1(p1), p2(p2)
		{
		}
		point &get_start_point()
		{
			return p1;
		}
		const point &get_start_point() const
		{
			return p1;
		}
		point &get_end_point()
		{
			return p2;
		}
		const point &get_end_point() const
		{
			return p2;
		}
		void draw(const paint_params &pp, const brush &brush, float width=1.f);
		void draw(const paint_params &pp, const brush *brush, float width=1.f)
		{
			draw(pp, *brush, width);
		}
	};

	struct circle{
		point center;
		FLOAT radius;

		circle(const point &p, FLOAT r): center(p), radius(r)
		{
		}
		point &get_center()
		{
			return center;
		}
		const point &get_center() const
		{
			return center;
		}
		FLOAT get_radius() const
		{
			return radius;
		}
		bool is_colliding(const circle &c)
		{
			return (center.x - c.center.x) * (center.x - c.center.x) + (center.y - c.center.y) * (center.y - c.center.y) <= (radius + c.radius) * (radius + c.radius);
		}
		void draw(const paint_params &pp, const brush &brush, float width=1.f);
		void draw(const paint_params &pp, const brush *brush, float width=1.f)
		{
			draw(pp, *brush, width);
		}
		void fill(const paint_params &pp, const brush &brush);
		void fill(const paint_params &pp, const brush *brush)
		{
			fill(pp, *brush);
		}
	};

	inline bool point::is_inside(const rect &r) const
	{
		return r.left <= x && x <= r.right && r.top <= y && y <= r.bottom;
	}

	inline rect operator+(rect r, point p)
	{
		return rect(point(r.left + p.x, r.top + p.y), size(r.width(), r.height()));
	}
	inline rect operator+(point p, rect r)
	{
		return r + p;
	}
	inline rect operator-(rect r, point p)
	{
		return rect(point(r.left - p.x, r.top - p.y), size(r.width(), r.height()));
	}
	inline rect operator+(rect r, size s)
	{
		return rect(r.left, r.top, r.right + s.width, r.bottom + s.height);
	}
	inline rect operator+(size s, rect r)
	{
		return r + s;
	}

	struct color: public D2D1::ColorF{
	public:
		color(Enum e): D2D1::ColorF(e)
		{
		}
		color(int r, int g, int b, int a=255): D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f)
		{
		}
		color(float r=0.f, float g=0.f, float b=0.f, float a=1.f): D2D1::ColorF(r, g, b, a)
		{
		}
		double get_luminance() const
		{
			return luminance(r, g, b);
		}
		double get_hue() const
		{
			return hue(r, g, b);
		}
		double get_saturation()
		{
			return saturation(r, g, b);
		}

		static double luminance(float r, float g, float b)
		{
			return 0.298912 * r + 0.586611 * g + 0.114478 * b;
		}
		static double hue(float r, float g, float b)
		{
			double u = - r * 0.1471376975169300226 - g * 0.2888623024830699774 + b * 0.436;
			double v =   r * 0.615 - g * 0.514985734664764622 - b * 0.100014265335235378;
			return std::atan2(v, u);
		}
		static double saturation(float r, float g, float b)
		{
			double u = - r * 0.1471376975169300226 - g * 0.2888623024830699774 + b * 0.436;
			double v =   r * 0.615 - g * 0.514985734664764622 - b * 0.100014265335235378;
			return std::sqrt(u * u + v * v);
		}
		static color from_hsl(double h, double s, double l, int a=255)
		{
			double u = cos(h) * s;
			double v = sin(h) * s;
			int r = static_cast<int>((l + 1.139837398373983740  * v) * 255);
			int g = static_cast<int>((l - 0.3946517043589703515  * u - 0.5805986066674976801 * v) * 255);
			int b = static_cast<int>((l + 2.03211091743119266 * u) * 255);
			return color(r, g, b, a);
		}
	};

	class polygon{
		factory factory;
		ID2D1PathGeometry *geometry;
		std::vector<point> points;
		point pos;

	public:
		polygon(): geometry(nullptr)
		{
		}

		polygon(std::initializer_list<point> pts): geometry(nullptr), points(pts)
		{
		}

		polygon(const polygon &p): geometry(nullptr), points(p.points)
		{
		}

		~polygon()
		{
			::quote::utils::SafeRelease(geometry);
		}

		void add_point(const point &p)
		{
			points.push_back(p);
		}

		void finalize()
		{
			if(geometry != nullptr)
				return;

			using namespace _com_util;

			CheckError(factory[D2D]->CreatePathGeometry(&geometry));

			_COM_SMARTPTR_TYPEDEF(ID2D1GeometrySink, __uuidof(ID2D1GeometrySink));
			ID2D1GeometrySinkPtr sink;
			CheckError(geometry->Open(&sink));
			sink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
			if(points.size() > 1)
				sink->AddLines(&points[1], points.size() - 1);
			sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			CheckError(sink->Close());
		}

		bool is_finalized() const
		{
			return geometry != nullptr;
		}

		void set_position(const point &p)
		{
			pos = p;
		}
		point get_position() const
		{
			return pos;
		}

		void draw(const paint_params &pp, const brush &brush, float width=1.f);
		void draw(const paint_params &pp, const brush *brush, float width=1.f)
		{
			draw(pp, *brush, width);
		}
		void fill(const paint_params &pp, const brush &brush);
		void fill(const paint_params &pp, const brush *brush)
		{
			fill(pp, *brush);
		}

		ID2D1PathGeometry *Get() const
		{
			return geometry;
		}
	};

	inline bool point::is_inside(const polygon &p) const
	{
		if(!p.is_finalized())
			return false;

		auto g = p.Get();
		auto pt = p.get_position();
		BOOL contains;
		_com_util::CheckError(g->FillContainsPoint(
			*this,
			D2D1::Matrix3x2F::Translation(pt.x, pt.y),
			&contains));

		return contains == TRUE;
	}

} }

#include "details/base_types.hpp"
