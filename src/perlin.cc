#include "perlin.h"
#include <random>
#include <iostream>
#include <algorithm>

Perlin::Perlin()
	:grid{}
{ }

void Perlin::init(int const grid_width, int const grid_height)
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
		}
	}

	for(int i{}; i < grid_height; ++i)
	{
		grid.push_back(std::vector<Vec2>{});
		for(int j{}; j < grid_width; ++j)
		{
			std::cout << grid.at(i).at(j) << ' ';
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

std::vector<std::vector<Vec2>> Perlin::get_grid() const
{
	return grid;
}
