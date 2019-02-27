#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using std::vector;
using std::sort;
using std::string;
using std::ostringstream;

enum PlacementType
{
	DIRECT_MAPPED, 
	TWO_WAY_SET_ASSOCIATIVE, 
	FOUR_WAY_SET_ASSOCIATIVE, 
	FULLY_ASSOCIATIVE
};
enum WritePolicy
{
	WRITE_BACK, 
	WRITE_THROUGH
};

struct Block
{
	size_t size;
	uint32_t tag;
	bool valid;
	size_t useTime;
	bool dirty;
	Block() : size(0), tag(0), valid(false), useTime(0), dirty(false) {}
	Block(size_t s, uint32_t t, bool v) : size(s), tag(t), valid(v), useTime(0), dirty(false) { }
	//Block(const Block & b) : size(b.size), tag(b.tag), valid(b.valid), useTimes(b.useTimes) { } 
	bool operator<(const Block & b) const
	{
		return this->useTime < b.useTime;
	}
	Block & operator=(const Block & b)
	{
		this->size = b.size;
		this->tag = b.tag;
		this->useTime = b.useTime;
		this->valid = b.valid;
		this->dirty = b.dirty;
		return *this;
	}
};

typedef vector<Block> Set;

class Cache
{
public:
	//Cache size, block size, Placement Type, Write Policy
	Cache(size_t s, size_t bs, PlacementType pt, WritePolicy wp);
	size_t getSize();
	size_t getBlockSize();
	bool readAddress(uint32_t ad);
	bool writeAddress(uint32_t ad);
	const float reportHitRate();
	const string getPlacementType();
	const string getWritePolicy();
	const size_t getCache2Mem();
	const size_t getMemtoCache();
	string toString();
	void clear();
private:
	size_t mem2Cache;
	size_t cache2Mem;
	size_t accessTimes;
	size_t hitTimes;
	size_t missTimes;
	const size_t blockSize;
	const size_t cacheSize;
	size_t numOfSets;
	PlacementType type;
	WritePolicy policy;
	vector<Set> data;
	void cacheMiss(uint32_t ad, bool WR);//true for W, false for R
	void evict(size_t way, size_t setIndex, uint32_t tag, bool WR);
	
};
