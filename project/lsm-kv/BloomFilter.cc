#include "BloomFilter.h"
#include "MurmurHash3.h"

#define BloomFilterSize 10240

void BloomFilter::BuildBloomFilter(std::list<std::pair<uint64_t, std::string>> &list, bool *bloomfilter)
{
    // init hash
    uint32_t hash[4] = {0};

    // build bloomfilter
    for (std::list<std::pair<uint64_t, std::string>>::iterator iter = list.begin(); iter != list.end(); iter++)
    {
        // get hash
        MurmurHash3_x64_128(&(*iter).first, sizeof(uint64_t), 1, hash);

        // set bloomfilter
        for (int i = 0; i < 4; i++)
        {
            bloomfilter[hash[i] % BloomFilterSize] = true;
        }
    }
}

bool BloomFilter::IsexitedinBloomFilter(uint64_t key, bool *bloomfilter)
{
    // init hash
    uint32_t hash[4] = {0};

    // get hash
    MurmurHash3_x64_128(&key, sizeof(uint64_t), 1, hash);

    for (int i = 0; i < 4; i++)
    {
        if (!bloomfilter[hash[i] % BloomFilterSize])
            return false;
    }
    return true;
}
