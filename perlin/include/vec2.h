#ifndef VEC2_H
#define VEC2_H

#include <ostream>

class Vec2
{
public:
	Vec2(float const x, float const y);

	float dot(Vec2 const& v) const;
	float mag() const;
	void normalize();
	float get_x() const;
	float get_y() const;

	Vec2 operator-(Vec2 const& rhs) const;
	Vec2 operator/(float const value) const;
	friend std::ostream& operator<<(std::ostream & os, Vec2 const& obj);

private:
	float x;
	float y;
};

#endif
