#include "vec2.h"

Vec2::Vec2(float const x, float const y)
	:x{x}, y{y}
{ }

float Vec2::dot(Vec2 const& v) const
{
	return this->x*v.x + this->y*v.y;
}

Vec2 Vec2::operator/(float const value) const
{
	return Vec2{this->x/value, this->y/value};
}

std::ostream& operator<<(std::ostream & os, Vec2 const& obj)
{
	return os << '(' << obj.x << ", " << obj.y << ')';
}
