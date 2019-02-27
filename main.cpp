#include "SA_cache.h"
#include <iostream>
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
int main(int argc, char *argv[])
{
	ifstream in;
	ofstream out;
	if (argc==3)
	{
		in = ifstream(argv[1]);
		out = ofstream(argv[2]);
	}
	else
	{
		in = ifstream("test.trace");
		out = ofstream("test.result");
	}
	string str(argv[2]);
	size_t cacheSizes[] = { 1024, 4096, 65536, 131072 };
	size_t blockSizes[] = { 8, 16, 32, 128 };
	PlacementType pt[] = { DIRECT_MAPPED, TWO_WAY_SET_ASSOCIATIVE, FOUR_WAY_SET_ASSOCIATIVE, FULLY_ASSOCIATIVE };
	vector<Cache> caches;
	caches.reserve(128);
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			for (size_t k = 0; k < 4; k++)
			{
				caches.push_back(Cache(cacheSizes[i], blockSizes[j], pt[k], WRITE_BACK));
				caches.push_back(Cache(cacheSizes[i], blockSizes[j], pt[k], WRITE_THROUGH));
			}
		}
	}
	Cache c(1024, 16, DIRECT_MAPPED, WRITE_BACK);
	string command;
	uint32_t ad;
	in >> command >> std::hex >> ad;
	while (!in.fail())
	{
		if (command == "read")
		{
			c.readAddress(ad);
			for (size_t i = 0; i < caches.size(); i++)
			{
				caches[i].readAddress(ad);
			}
		}
		else if (command == "write")
		{
			c.writeAddress(ad);
			for (size_t i = 0; i < caches.size(); i++)
			{
				caches[i].writeAddress(ad);
			}
		}
		in >> command >> std::hex >> ad;
	}
	for (size_t i = 0; i < caches.size(); i++)
	{
		if (caches[i].getWritePolicy() == "WB")
		{
			caches[i].clear();
		}
		
		out.precision(2);
		out << caches[i].toString();
	}
	//cout << "Test\n" << c.toString();

	in.close();
	out.close();
	return EXIT_SUCCESS;
}