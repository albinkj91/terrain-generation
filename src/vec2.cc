#include "vec2.h"
#include <cmath>

Vec2::Vec2(float const x, float const y)
	:x{x}, y{y}
{ }

float Vec2::dot(Vec2 const& v) const
{
	return x*v.x + y*v.y;
}

float Vec2::mag() const
{
	return sqrt(x*x + y*y);
}

void Vec2::normalize()
{
	*this / mag();
}

Vec2 Vec2::operator-(Vec2 const& rhs) const
{
	return Vec2{x - rhs.x, y - rhs.y};
}

Vec2 Vec2::operator/(float const value) const
{
	return Vec2{x / value, y / value};
}

std::ostream& operator<<(std::ostream & os, Vec2 const& obj)
{
	return os << '(' << obj.x << ", " << obj.y << ')';
}
