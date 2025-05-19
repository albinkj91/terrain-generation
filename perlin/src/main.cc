#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include "perlin.h"
using namespace std;

#define byte unsigned char
#define img_width 512
#define cell_width 8

void write_ppm(int const width, int const height, vector<vector<float>> bytes)
{
	fstream output{"perlin.ppm", ios_base::out};
	output << "P6" << '\n';
	output << width << ' ' << height << '\n';
	output << 255 << '\n';

	for_each(bytes.begin(), bytes.end(),
			[&output](vector<float> const& b)
			{
				copy(b.begin(), b.end(), ostream_iterator<float>{output, " "});
			});
}

int main()
{
	Perlin p{cell_width, cell_width, img_width, img_width};
	Perlin p2{cell_width*2, cell_width*2, img_width, img_width};
	Perlin p3{cell_width*4, cell_width*4, img_width, img_width};
	Perlin p4{cell_width*8, cell_width*8, img_width, img_width};
	Perlin p5{cell_width*16, cell_width*16, img_width, img_width};

	p.init();
	p2.init();
	p3.init();
	p4.init();
	p5.init();

	vector<vector<float>> bytes{};
	for(int i{}; i < img_width; ++i)
	{
		bytes.push_back(vector<float>{});
		for(int j{}; j < img_width; ++j)
		{
			float added_noise{p.noise_2d(j, i) +
				p2.noise_2d(j, i)/2.0f +
				p3.noise_2d(j, i)/4.0f +
				p4.noise_2d(j, i)/8.0f +
				p5.noise_2d(j, i)/16.0f
			};

			if(added_noise < 0.9f)
				added_noise = 0.9f;
			bytes.at(i).push_back(added_noise);
		}
	}
	write_ppm(img_width, img_width, bytes);
}
