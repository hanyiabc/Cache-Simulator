#include "SA_cache.h"
Cache::Cache(size_t s, size_t bs, PlacementType pt, WritePolicy wp) : cacheSize(s), type(pt), policy(wp), blockSize(bs), accessTimes(0), missTimes(0), hitTimes(0), mem2Cache(0), cache2Mem(0)
{ 
switch (pt)																										
	{
	case DIRECT_MAPPED:
		numOfSets = cacheSize / blockSize;
		break;
	case TWO_WAY_SET_ASSOCIATIVE:
		numOfSets = cacheSize / blockSize / 2;
		break;
	case FOUR_WAY_SET_ASSOCIATIVE:
		numOfSets = cacheSize / blockSize / 4;
		break;
	case FULLY_ASSOCIATIVE:
		numOfSets = 1;
		break;
	default:
		break;
	}

	data.reserve(numOfSets);

	for (size_t i = 0; i < numOfSets; i++)
	{
		size_t ways = 0;
		data.push_back(Set());
		switch (pt)
		{
		case DIRECT_MAPPED:
			data.back().reserve(1);
			ways = 1;
			break;
		case TWO_WAY_SET_ASSOCIATIVE:
			data.back().reserve(2);
			ways = 2;
			break;
		case FOUR_WAY_SET_ASSOCIATIVE:
			data.back().reserve(4);
			ways = 4;
			break;
		case FULLY_ASSOCIATIVE:
			ways = cacheSize / blockSize;
			data.back().reserve(ways);
			break;
		default:
			break;
		}
		for (size_t j = 0; j < ways; j++)
		{
			data.back().push_back(Block(this->blockSize, 0, false));
		}
		
	}
	
}


size_t Cache::getSize()
{
	return this->cacheSize;
}

size_t Cache::getBlockSize()
{
	return blockSize;
}

bool Cache::readAddress(uint32_t ad)
{
	size_t setIndex = (ad / blockSize) % numOfSets;
	uint32_t tag = ad / (numOfSets *  blockSize);
	this->accessTimes++;
	for (size_t i = 0; i < data[setIndex].size(); i++)
	{
		if (data[setIndex][i].valid && data[setIndex][i].tag == tag)
		{
			this->hitTimes++;
			data[setIndex][i].useTime = accessTimes;
			return true;//cache hit
		}
	}

	cacheMiss(ad, false);
	return false;//miss
}

bool Cache::writeAddress(uint32_t ad)
{
	if (policy==WRITE_THROUGH)
	{
		this->cache2Mem += 4;
	}

	size_t setIndex = (ad / blockSize) % numOfSets;
	uint32_t tag = ad / (numOfSets *  blockSize);
	this->accessTimes++;
	for (size_t i = 0; i < data[setIndex].size(); i++)
	{
		if (data[setIndex][i].valid && data[setIndex][i].tag == tag)
		{
			data[setIndex][i].useTime = accessTimes;
			if (policy == WRITE_BACK)
			{
				data[setIndex][i].dirty = true;
			}
			this->hitTimes++;
			return true;//cache hit
		}
	}
	cacheMiss(ad, true);
	return false;//miss
}

void Cache::cacheMiss(uint32_t ad, bool WR)
{
	this->missTimes++;
	size_t setIndex = (ad / blockSize) % numOfSets;
	uint32_t tag = ad / (numOfSets *  blockSize);
	sort(data[setIndex].begin(), data[setIndex].end());
	this->mem2Cache += blockSize;
	evict(0, setIndex, tag, WR);
}

void Cache::evict(size_t way, size_t setIndex, uint32_t tag, bool WR)
{
	data[setIndex][way].valid = true;
	data[setIndex][way].tag = tag;
	data[setIndex][way].useTime = accessTimes;
	if (policy == WRITE_BACK && data[setIndex][way].dirty && WR)
	{
		cache2Mem += blockSize;
	}
	else if (policy == WRITE_BACK && data[setIndex][way].dirty && !WR)
	{
		cache2Mem += blockSize;
		data[setIndex][way].dirty = false;
	}
	if (WR)
	{
		data[setIndex][way].dirty = true;
	}
}

void Cache::clear()
{
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			if (data[i][j].dirty)
			{
				cache2Mem += blockSize;
			}
		}
	}
}

const float Cache::reportHitRate()
{
	if (accessTimes==0)
	{
		return -1;
	}
	else
	{
		return ((float)hitTimes / (float)accessTimes);
	}
}

const string Cache::getPlacementType()
{
	switch (type)
	{
	case DIRECT_MAPPED:
		return "DM";
		break;
	case TWO_WAY_SET_ASSOCIATIVE:
		return "2W";
		break;
	case FOUR_WAY_SET_ASSOCIATIVE:
		return "4W";
		break;
	case FULLY_ASSOCIATIVE:
		return "FA";
		break;
	default:
		return "ERROR!";
		break;
	}
}

const string Cache::getWritePolicy()
{
	switch (policy)
	{
	case WRITE_BACK:
		return "WB";
		break;
	case WRITE_THROUGH:
		return "WT";
		break;
	default:
		return "ERROR!";
		break;
	}
}

const size_t Cache::getCache2Mem()
{
	return cache2Mem;
}

const size_t Cache::getMemtoCache()
{
	return mem2Cache;
}

string Cache::toString()
{
	ostringstream os;
	os.precision(2);
	os << getSize() << '\t' << getBlockSize() << '\t' << getPlacementType() << '\t' << getWritePolicy() << '\t' << std::fixed << reportHitRate() << '\t' << getMemtoCache() << '\t' << getCache2Mem() << '\n';
	return  os.str();
}
