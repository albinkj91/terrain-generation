#ifndef PERLIN_H
#define PERLIN_H

#include "vec2.h"
#include <vector>

class Perlin
{
public:
	Perlin(int const grid_w, int const grid_h, int const img_w, int const img_h);
	void init();
	float noise_2d(int const x, int const y) const;
private:
	std::vector<std::vector<Vec2>> grid;
	int grid_w;
	int grid_h;
	int img_w;
	int img_h;

	float smoothstep(float const x) const;
};

#endif
