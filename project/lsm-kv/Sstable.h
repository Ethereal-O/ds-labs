#pragma once

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <list>
#include <vector>
#include <string>

#define NOT_FOUND ""
#define DELETED "~DELETED~"
#define PRE_SIZE 32 + 10240
#define MAX_SIZE 2 * 1024 * 1024
#define BloomFilterSize 10240
#define MAXCONTAIN(levelnum) 2 * (levelnum + 1)
//#define MAXCONTAIN(levelnum) 10000

class Sstable
{
private:
    // STRUCTS and VARIATES
    // sstable buffer struct
    struct SSBuffer
    {
        std::string filename;
        uint64_t timestamp, num, minkey, maxkey;
        bool *BloomFilter;
        std::vector<std::pair<uint64_t, uint32_t>> data;
        SSBuffer(uint64_t ttimestamp, uint64_t tnum, uint64_t tminkey, uint64_t tmaxkey) : timestamp(ttimestamp), num(tnum), minkey(tminkey), maxkey(tmaxkey){};
    };

    // sslevel buffer struct
    struct SSLevel
    {
        uint64_t levelnum;
        uint64_t sstablenum;
        std::vector<struct SSBuffer *> buffer;
        struct SSLevel *nextlevel;
        SSLevel(uint64_t lnum = 0) : levelnum(lnum), sstablenum(0), nextlevel(NULL){};
    };

    // val data
    struct DataInfo
    {
        uint64_t foundlevelnum;
        uint64_t foundbuffer;
        uint64_t dataoffset;
        uint64_t valuelength;
    };

    // first level point
    struct SSLevel *level;

    // levelnum
    uint64_t leveltotnum;

    // store position
    const std::string storepos;

    // PRIVATE FUNCTIONS
    // initialize
    uint64_t InitializeLevelT(struct SSLevel *nlevel);

    // build
    void BuildSstable(uint64_t &timestamp, uint64_t &num, uint64_t &minkey, uint64_t &maxkey, std::list<std::pair<uint64_t, std::string>> &list, std::string filename, struct SSLevel *nlevel);

    // search
    bool Isexited(uint64_t key, struct DataInfo &founddatainfo);
    bool IsexitedinLevel0(uint64_t key, uint64_t &foundbuffer, uint64_t &dataoffset, uint64_t &valuelength);
    bool IsexitedinLevelT(uint64_t key, struct SSLevel *nlevel, uint64_t &foundbuffer, uint64_t &dataoffset, uint64_t &valuelength);

    // scan
    void GetScanLevelT(uint64_t key1, uint64_t key2, struct SSLevel *nlevel, std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs);
    void GetScanBuffer(uint64_t key1, uint64_t key2, uint64_t levelnum, uint64_t buffernum, struct SSBuffer *buffer, std::list<std::pair<uint64_t, struct DataInfo>> &bufferdatalist);

    // compaction
    void CompactionLevel(struct SSLevel *nlevel);
    void GetCompactionBuffer(bool isnextlevel, struct SSLevel *nlevel, uint64_t &minkey, uint64_t &maxkey, std::vector<uint64_t> &bufferlist);
    void GetCompactionAllData(std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs, std::vector<uint64_t> &bufferlist, struct SSLevel *nlevel);

    // delete
    void DeleteSelectedBufferandSstable(struct SSLevel *nlevel, std::vector<uint64_t> &selectedbufferlist);

    // replace
    void Replace(std::list<std::pair<uint64_t, struct DataInfo>> &bufferdatalist, std::list<std::pair<uint64_t, std::string>> &sstabledatalist);

    // sort
    void Mergesort(bool isScanorLastlevel, std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs, std::list<std::pair<uint64_t, std::string>> &list);

public:
    // PUBLIC FUNCTIONS
    // constructor
    Sstable(const std::string &dir);

    // initialize
    uint64_t InitializeSstable();

    // build
    void BuildSstableFromSkipList(uint64_t &timestamp, uint64_t &num, uint64_t &minkey, uint64_t &maxkey, std::list<std::pair<uint64_t, std::string>> &list);

    // search
    std::string SearchSstable(uint64_t key);

    // scan
    void ScanSstable(uint64_t key1, uint64_t key2, uint64_t skiplisttimestamp, std::list<std::pair<uint64_t, std::string>> &slist, std::list<std::pair<uint64_t, std::string>> &list);

    // compaction
    void Compaction();

    // delete
    void DeleteAllSstable();
    void DeleteAllBuffer();

    // reset
    void Reset();
};