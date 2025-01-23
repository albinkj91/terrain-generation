#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <vec2.h>
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
	vector<vector<byte>> bytes{};
	for(int i{}; i < 256; ++i)
	{
		bytes.push_back(vector<byte>{});
		for(int j{}; j < 256; ++j)
		{
			bytes.at(i).push_back(j);
			bytes.at(i).push_back(i);
			bytes.at(i).push_back(255-j);
		}
	}
	write_ppm(256, 256, bytes);

	Vec2 v1{1.5f, 2.7f};
}
