#ifndef PERLIN_H
#define PERLIN_H

#include "vec2.h"
#include <vector>

class Perlin
{
public:
	Perlin(int const grid_w, int const grid_h, int const point_w, int const point_h);
	void init();
	float noise_2d(int const x, int const y) const;
private:
	std::vector<std::vector<Vec2>> grid;
	int grid_width;
	int grid_height;
	int point_width;
	int point_height;
};

#endif
