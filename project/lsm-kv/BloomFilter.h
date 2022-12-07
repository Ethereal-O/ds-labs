#pragma once

#include <cstdint>
#include <list>
#include <string>

class BloomFilter
{
private:
public:
    static void BuildBloomFilter(std::list<std::pair<uint64_t, std::string>> &list, bool *bloomfilter);
    static bool IsexitedinBloomFilter(uint64_t key, bool *bloomfilter);
};