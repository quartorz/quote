#pragma once

#include "matrix.hpp"

#include <quote/direct2d/base_types.hpp>
namespace paint = quote::direct2d;

#include <cmath>

float to_rad(float deg)
{
	return deg * 3.1415926535f / 180.f;
}

float to_deg(float rad)
{
	return rad * 180.f / 3.1415926535f;
}

using vector = matrix<2, 1>;

class transform: public matrix<2, 3>{
public:
	template <class Element>
	transform(const std::initializer_list<Element> &list): matrix<2, 3>(list)
	{
	}
	transform()
	{
	}
	template <class... Elements, class = std::enable_if<sizeof...(Elements) != 0>::type>
	transform(Elements... args): transform({args...})
	{
	}
	transform operator*(const transform &t)
	{
		return transform(
			(*this)[0][0] * t[0][0] + (*this)[0][1] * t[1][0],
			(*this)[0][0] * t[0][1] + (*this)[0][1] * t[1][1],
			(*this)[0][0] * t[0][2] + (*this)[0][1] * t[1][2] + (*this)[0][2],
			(*this)[1][0] * t[0][0] + (*this)[1][1] * t[1][0],
			(*this)[1][0] * t[0][1] + (*this)[1][1] * t[1][1],
			(*this)[1][0] * t[0][2] + (*this)[1][1] * t[1][2] + (*this)[1][2]
		);
	}
	vector operator*(const vector &v)
	{
		return vector(
			(*this)[0][0] * v[0][0] + (*this)[0][1] * v[1][0] + (*this)[0][2],
			(*this)[1][0] * v[0][0] + (*this)[1][1] * v[1][0] + (*this)[1][2]
		);
	}
	static transform identity()
	{
		return transform(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f
		);
	}
	static transform translation(float x, float y)
	{
		return transform(
			1.f, 0.f, x,
			0.f, 1.f, y
		);
	}
	static transform translation(paint::point p)
	{
		return translation(p.x, p.y);
	}
	// theta: x軸の正方向からy軸の正方向に向かう角度
	static transform rotation(float theta)
	{
		float cos = std::cos(theta);
		float sin = std::sin(theta);
		return transform(
			cos, -sin, 0.f,
			sin, cos, 0.f
		);
	}
	static transform scale(float x, float y)
	{
		return transform(
			x, 0.f, 0.f,
			0.f, y, 0.f
		);
	}
};

class character{
	float radius = 1.f;

	// 現在位置からの移動ベクトル
	vector vector_;

	paint::point pos;
	paint::size size;

public:
	paint::point get_center() const
	{
		return pos + paint::point(size.width / 2, size.height / 2);
	}

	void set_position(const paint::point &p)
	{
		pos = p;
	}
	paint::point get_position() const
	{
		return pos;
	}
	void set_size(const paint::size &s)
	{
		size = s;
	}
	paint::size get_size() const
	{
		return size;
	}

	void set_radius(float r)
	{
		radius = r;
	}

	float get_radius() const
	{
		return radius;
	}

	paint::circle get_circle() const
	{
		return paint::circle(get_center(), radius);
	}

	bool is_colliding(const character &c)
	{
		return get_circle().is_colliding(c.get_circle());
	}

	void move()
	{
		set_position(paint::point(pos.x + vector_[0][0], pos.y + vector_[1][0]));
	}
	void set_vector(const vector &v)
	{
		vector_ = v;
	}
	vector get_vector() const
	{
		return vector_;
	}
};
