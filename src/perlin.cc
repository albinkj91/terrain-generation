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
	Vec2 point{static_cast<float>(x), static_cast<float>(y)};
	int offset_x{point_width / grid_width};
	int offset_y{point_height / grid_height};
	int grid_x{x / offset_x};
	int grid_y{y / offset_y};

	std::cout << "Point w: " << point_width << '\n';
	std::cout << "Point h: " << point_height << '\n';
	std::cout << "Grid w: " << grid_width << '\n';
	std::cout << "Grid h: " << grid_height << '\n';
	std::cout << "x: " << x << '\n';
	std::cout << "y: " << y << '\n';

	std::cout << "Offset x: " << offset_x << '\n';
	std::cout << "Offset y: " << offset_y << '\n';
	std::cout << "Point grid x: " << grid_x << '\n';
	std::cout << "Point grid y: " << grid_y << '\n';

	Vec2 offset_1{point.get_x(), point.get_y()};
	Vec2 offset_2{point.get_x() - (offset_x-1), point.get_y()};
	Vec2 offset_3{point.get_x(), point.get_y() - (offset_y-1)};
	Vec2 offset_4{point.get_x() - (offset_x-1), point.get_y() - (offset_y-1)};

	std::cout << "offset 1: " << offset_1 << '\n';
	std::cout << "offset 2: " << offset_2 << '\n';
	std::cout << "offset 3: " << offset_3 << '\n';
	std::cout << "offset 4: " << offset_4 << '\n';

	float dot_1{offset_1.dot(grid.at(grid_y).at(grid_x))};
	float dot_2{offset_2.dot(grid.at(grid_y).at(grid_x+1))};
	float dot_3{offset_3.dot(grid.at(grid_y+1).at(grid_x))};
	float dot_4{offset_4.dot(grid.at(grid_y+1).at(grid_x+1))};

	std::cout << "dot 1: " << dot_1 << '\n';
	std::cout << "dot 2: " << dot_2 << '\n';
	std::cout << "dot 3: " << dot_3 << '\n';
	std::cout << "dot 4: " << dot_4 << '\n';

	float lerp_1{dot_1 + smoothstep(x % offset_x) * (dot_2-dot_1)};
	float lerp_2{dot_3 + smoothstep(x % offset_x) * (dot_4-dot_3)};
	float lerp_3{lerp_1 + smoothstep(y % offset_y) * (lerp_2-lerp_1)};

	std::cout << "Final result: " << lerp_3 << std::endl;
	return lerp_3;
}

float Perlin::smoothstep(float const x) const
{
	if(x <= 0)
		return 0;
	else if(x >= 0 && x <= 1)
		return 3*x*x - 2*x*x*x;
	return 1;
}
