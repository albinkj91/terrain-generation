#ifndef PERLIN_H
#define PERLIN_H

#include "vec2.h"
#include <vector>

class Perlin
{
public:
	Perlin();
	void init(int const grid_width, int const grid_height);
	float get(int const x, int const y) const;
private:
	std::vector<std::vector<Vec2>> grid;
};

#endif
