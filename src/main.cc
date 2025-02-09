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

void write_ppm(int const width, int const height, vector<vector<byte>> bytes)
{
	fstream output{"perlin.ppm", ios_base::out};
	output << "P6" << '\n';
	output << width << ' ' << height << '\n';
	output << 255 << '\n';

	for_each(bytes.begin(), bytes.end(),
			[&output](vector<byte> const& b)
			{
				copy(b.begin(), b.end(), ostream_iterator<byte>(output));
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

	vector<vector<byte>> bytes{};
	for(int i{}; i < img_width; ++i)
	{
		bytes.push_back(vector<byte>{});
		for(int j{}; j < img_width; ++j)
		{
			float added_noise{p.noise_2d(j, i) +
				p2.noise_2d(j, i)/2.0f +
				p3.noise_2d(j, i)/4.0f +
				p4.noise_2d(j, i)/8.0f +
				p5.noise_2d(j, i)/16.0f -
				0.5f
			};

			byte val{static_cast<byte>(added_noise * 255.0f)};
			bytes.at(i).push_back(val);
			bytes.at(i).push_back(val);
			bytes.at(i).push_back(val);
		}
	}
	write_ppm(img_width, img_width, bytes);
}
