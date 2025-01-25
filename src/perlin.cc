#include "perlin.h"
#include <random>
#include <iostream>
#include <algorithm>

Perlin::Perlin(int const grid_w, int const grid_h, int const point_w, int const point_h)
	:grid{}, grid_width{grid_w}, grid_height{grid_h}, point_width{point_w}, point_height{point_h}
{ }

void Perlin::init()
{
	std::random_device rand{};
	std::mt19937 gen{rand()};
	std::uniform_real_distribution<> dis{0.0, 1.0};

	for(int i{}; i < grid_height; ++i)
	{
		grid.push_back(std::vector<Vec2>{});
		for(int j{}; j < grid_width; ++j)
		{
			grid.at(i).push_back(Vec2{static_cast<float>(dis(gen)), static_cast<float>(dis(gen))});
			grid.at(i).at(j).normalize();
		}
	}
}

float Perlin::noise_2d(int const x, int const y) const
{
	int grid_x{x / grid_width};
	int grid_y{y / grid_height};
	return grid_x+grid_y;
}
