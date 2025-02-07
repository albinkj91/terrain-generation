#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include "perlin.h"
using namespace std;

#define byte unsigned char

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
	Perlin p{16, 16, 512, 512};
	p.init();
	vector<vector<byte>> bytes{};
	for(int i{}; i < 512; ++i)
	{
		bytes.push_back(vector<byte>{});
		for(int j{}; j < 512; ++j)
		{
			byte val{static_cast<byte>(p.noise_2d(j, i) * 255)};
			bytes.at(i).push_back(val);
			bytes.at(i).push_back(val);
			bytes.at(i).push_back(val);
		}
	}
	write_ppm(512, 512, bytes);
}
