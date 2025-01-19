#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

#define byte unsigned char

void write_ppm(int const width, int const height, vector<vector<byte>> bytes)
{
	fstream output{"output.ppm", ios_base::out | ios_base::binary};
	// From: https://netpbm.sourceforge.net/doc/ppm.html
	output << "P6" << '\n';
	output << width << ' ' << height << '\n';
	output << 255 << '\n';

	for(size_t i{}; i < bytes.size(); ++i)
	{
		for(size_t j{}; j < bytes.at(i).size(); j += 3)
		{
			output << bytes.at(i).at(j)
				<< bytes.at(i).at(j+1)
				<< bytes.at(i).at(j+2);
		}
	}
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
			bytes.at(i).push_back(255 - j);
		}
	}
	write_ppm(256, 256, bytes);
}
