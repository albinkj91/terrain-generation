#include "perlin.h"
#include <random>
#include <algorithm>
#include <iostream>

Perlin::Perlin(int const grid_w, int const grid_h, int const point_w, int const point_h)
	:grid{}, grid_width{grid_w}, grid_height{grid_h}, point_width{point_w}, point_height{point_h}
{ }

void Perlin::init()
{
	std::random_device rand{};
	std::mt19937 gen{rand()};
	std::uniform_real_distribution<> dis{-1.0, 1.0};

	for(int i{}; i <= grid_height; ++i)
	{
		grid.push_back(std::vector<Vec2>{});
		for(int j{}; j <= grid_width; ++j)
		{
			grid.at(i).push_back(Vec2{static_cast<float>(dis(gen)), static_cast<float>(dis(gen))});
		}
	}
}

float Perlin::noise_2d(int const x, int const y) const
{
	int cell_w{point_width / grid_width};
	int cell_h{point_height / grid_height};

	int rel_x{x % cell_w};
	int rel_y{y % cell_h};

	int grid_x{x / cell_w};
	int grid_y{y / cell_h};

	float normalized_x{static_cast<float>(rel_x) / static_cast<float>(cell_w)};
	float normalized_y{static_cast<float>(rel_y) / static_cast<float>(cell_h)};

	Vec2 point{normalized_x, normalized_y};

	Vec2 offset_1{point.get_x(), point.get_y()};
	Vec2 offset_2{point.get_x() - 1.0f, point.get_y()};
	Vec2 offset_3{point.get_x(), point.get_y() - 1.0f};
	Vec2 offset_4{point.get_x() - 1.0f, point.get_y() - 1.0f};

	float dot_1{offset_1.dot(grid.at(grid_y).at(grid_x))};
	float dot_2{offset_2.dot(grid.at(grid_y).at(grid_x+1))};
	float dot_3{offset_3.dot(grid.at(grid_y+1).at(grid_x))};
	float dot_4{offset_4.dot(grid.at(grid_y+1).at(grid_x+1))};

	float lerp_1{dot_1 + smoothstep(point.get_x()) * (dot_2-dot_1)};
	float lerp_2{dot_3 + smoothstep(point.get_x()) * (dot_4-dot_3)};
	float lerp_3{lerp_1 + smoothstep(point.get_y()) * (lerp_2-lerp_1)};

	lerp_3 += 0.5f;

	if(lerp_3 >= 1.0f)
		return 1.0f;
	if(lerp_3 <= 0.0f)
		return 0.0f;
	return lerp_3;
}

float Perlin::smoothstep(float const x) const
{
	if(x <= 0.0f)
		return 0.0f;
	else if(x >= 0.0f && x <= 1.0f)
		return 3.0f*x*x - 2.0f*x*x*x;
	return 1.0f;
}
