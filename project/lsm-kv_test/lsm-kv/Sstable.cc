#include "Sstable.h"
#include "utils.h"
#include "BloomFilter.h"

// constructor
Sstable::Sstable(const std::string &dir) : storepos(dir)
{
    level = new Sstable::SSLevel(0);
    leveltotnum = 0;
}

// initialize
uint64_t Sstable::InitializeSstable()
{
    uint64_t maxtimestamp = 0;
    // judge if first level exists
    if (!utils::dirExists(storepos + "/level-0/"))
        return maxtimestamp;

    // initialize first level
    maxtimestamp = std::max(InitializeLevelT(level), maxtimestamp);
    leveltotnum = 1;

    // initialize other level
    struct SSLevel *plevel = level;
    for (int i = 1;; i++)
    {
        // judge if this level exists
        if (!utils::dirExists(storepos + "/level-" + std::to_string(i) + "/"))
            break;

        // initialize this level
        struct SSLevel *nlevel = new SSLevel(i);
        maxtimestamp = std::max(InitializeLevelT(nlevel), maxtimestamp);

        // link and set leveltotnum
        plevel->nextlevel = nlevel;
        leveltotnum = i + 1;
        plevel = nlevel;
    }

    // return premaxtimestamp;
    return maxtimestamp;
}

uint64_t Sstable::InitializeLevelT(struct SSLevel *nlevel)
{
    // initialize and get all name
    uint64_t maxtimestamp = 0;
    nlevel->sstablenum = 0;
    std::vector<std::string> buffername;
    utils::scanDir(storepos + "/level-" + std::to_string(nlevel->levelnum) + "/", buffername);

    for (std::vector<std::string>::iterator iter = buffername.begin(); iter != buffername.end(); iter++)
    {
        // read file
        std::ifstream file(storepos + "/level-" + std::to_string(nlevel->levelnum) + "/" + (*iter));

        // create data
        uint64_t timestamp = 0, num = 0, minkey = 0, maxkey = 0;
        bool *BloomFilter = new bool[BloomFilterSize];
        std::vector<std::pair<uint64_t, uint32_t>> data;

        // read header
        file.read((char *)&timestamp, sizeof(uint64_t));
        file.read((char *)&num, sizeof(uint64_t));
        file.read((char *)&minkey, sizeof(uint64_t));
        file.read((char *)&maxkey, sizeof(uint64_t));

        // read BloomFilter
        for (int i = 0; i < BloomFilterSize; i++)
        {
            char tmpch;
            file.read(&tmpch, 1);
            if (tmpch == '0')
            {
                BloomFilter[i] = false;
            }
            else
            {
                BloomFilter[i] = true;
            }
        }

        // read link
        for (int i = 0; i < num; i++)
        {
            uint64_t key;
            uint32_t offset;
            file.read((char *)&key, sizeof(uint64_t));
            file.read((char *)&offset, sizeof(uint32_t));
            data.push_back(std::make_pair(key, offset));
        }

        // close file
        file.close();

        // write to buffer
        SSBuffer *newbuffer = new SSBuffer(timestamp, num, minkey, maxkey);
        newbuffer->filename = (*iter).substr(0, (*iter).length() - 4);
        newbuffer->BloomFilter = BloomFilter;
        newbuffer->data = data;

        // write to level
        nlevel->sstablenum++;
        nlevel->buffer.push_back(newbuffer);
        maxtimestamp = std::max(maxtimestamp, timestamp);
    }
    return maxtimestamp;
}

// build
void Sstable::BuildSstableFromSkipList(uint64_t &timestamp, uint64_t &num, uint64_t &minkey, uint64_t &maxkey, std::list<std::pair<uint64_t, std::string>> &list)
{
    // create dir
    if (!utils::dirExists(storepos + "/level-0/"))
        utils::mkdir((storepos + "/level-0/").c_str());

    // build sstable from skiplist
    BuildSstable(timestamp, num, minkey, maxkey, list, std::to_string(timestamp) + "-1", level);

    // Set levelnum and Compaction
    if (leveltotnum == 0)
        leveltotnum = 1;
    Compaction();
}

void Sstable::BuildSstable(uint64_t &timestamp, uint64_t &num, uint64_t &minkey, uint64_t &maxkey, std::list<std::pair<uint64_t, std::string>> &list, std::string filename, struct SSLevel *nlevel)
{
    // create newbuffer
    struct SSBuffer *newbuffer = new SSBuffer(timestamp, num, minkey, maxkey);
    newbuffer->filename = filename;
    nlevel->buffer.push_back(newbuffer);
    nlevel->sstablenum++;

    // Header
    std::ofstream file(storepos + "/level-" + std::to_string(nlevel->levelnum) + "/" + newbuffer->filename + ".sst");
    file.write((char *)&timestamp, sizeof(uint64_t));
    file.write((char *)&num, sizeof(uint64_t));
    file.write((char *)&minkey, sizeof(uint64_t));
    file.write((char *)&maxkey, sizeof(uint64_t));

    // BloomFilter
    bool *BloomFilter = new bool[BloomFilterSize];
    memset(BloomFilter, 0, sizeof(bool) * BloomFilterSize);
    BloomFilter::BuildBloomFilter(list, BloomFilter);
    for (int i = 0; i < BloomFilterSize; i++)
    {
        file << BloomFilter[i];
    }
    newbuffer->BloomFilter = BloomFilter;

    // Link
    uint32_t nowaddr = 0;
    uint64_t length;
    for (std::list<std::pair<uint64_t, std::string>>::const_iterator iter = list.begin(); iter != list.end(); iter++)
    {
        length = (*iter).second.length();
        file.write((char *)(&(*iter).first), sizeof(uint64_t));
        file.write((char *)(&nowaddr), sizeof(uint32_t));
        newbuffer->data.push_back(std::make_pair((*iter).first, nowaddr));
        nowaddr += length;
    }

    // Data
    for (std::list<std::pair<uint64_t, std::string>>::const_iterator iter = list.begin(); iter != list.end(); iter++)
    {
        file << (*iter).second;
    }
    file.close();
}

// search
std::string Sstable::SearchSstable(uint64_t key)
{
    /*
    * Here is test for not use buffer
    * Below is really function which has buffer
    */
    // struct DataInfo founddatainfo;
    // uint64_t maxtimestamp = 0,maxnum;
    // std::string foundbuffername;
    // for (int j = 0;; j++)
    // {
    //     // judge if this level exists
    //     std::string dirname = storepos + "/level-" + std::to_string(j) + "/";
    //     if (!utils::dirExists(dirname))
    //         break;

    //     std::vector<std::string> buffername;
    //     utils::scanDir(dirname, buffername);

    //     for (std::vector<std::string>::iterator iter = buffername.begin(); iter != buffername.end(); iter++)
    //     {
    //         // read file
    //         std::ifstream file(dirname + (*iter));

    //         // create data
    //         uint64_t timestamp = 0, num = 0, minkey = 0, maxkey = 0;
    //         bool *BloomFilter = new bool[BloomFilterSize];

    //         // read header
    //         file.read((char *)&timestamp, sizeof(uint64_t));
    //         file.read((char *)&num, sizeof(uint64_t));
    //         file.read((char *)&minkey, sizeof(uint64_t));
    //         file.read((char *)&maxkey, sizeof(uint64_t));

    //         // read BloomFilter
    //         for (int i = 0; i < BloomFilterSize; i++)
    //         {
    //             char tmpch;
    //             file.read(&tmpch, 1);
    //             if (tmpch == '0')
    //             {
    //                 BloomFilter[i] = false;
    //             }
    //             else
    //             {
    //                 BloomFilter[i] = true;
    //             }
    //         }
    //         delete []BloomFilter;

    //         // read link
    //         for (int i = 0; i < num; i++)
    //         {
    //             uint64_t tkey;
    //             uint32_t toffset;
    //             file.read((char *)&tkey, sizeof(uint64_t));
    //             file.read((char *)&toffset, sizeof(uint32_t));
    //             if (tkey == key&&timestamp>maxtimestamp)
    //             {
    //                 founddatainfo.foundlevelnum = j;
    //                 founddatainfo.dataoffset = toffset;
    //                 foundbuffername = (*iter);
    //                 if (i==num-1)
    //                 {
    //                     founddatainfo.valuelength = 0;
    //                 }else{
    //                     i++;
    //                     uint32_t t2offset;
    //                     file.read((char *)&tkey, sizeof(uint64_t));
    //                     file.read((char *)&t2offset, sizeof(uint32_t));
    //                     founddatainfo.valuelength = t2offset-toffset;
    //                 }
    //                 maxtimestamp = timestamp;
    //                 maxnum=num;
    //             }
    //         }

    //         // close file
    //         file.close();
    //     }
    // }
    // if (maxtimestamp==0)
    // {
    //     return NOT_FOUND;
    // }

    // // read from file
    // // struct SSLevel *tlevel = level;
    // // for (int i = 0; i < founddatainfo.foundlevelnum; i++)
    // // {
    // //     tlevel = tlevel->nextlevel;
    // // }
    // std::fstream file(storepos + "/level-" + std::to_string(founddatainfo.foundlevelnum) + "/" + foundbuffername);

    // // read predata
    // uint64_t prenum = PRE_SIZE + maxnum * (sizeof(uint64_t) + sizeof(uint32_t)) + founddatainfo.dataoffset;


    /*
    * Here is really function which has buffer
    * Above is test for not use buffer
    */
    // found from buffer
    struct DataInfo founddatainfo;
    if (!Isexited(key, founddatainfo))
        return NOT_FOUND;

    // read from file
    struct SSLevel *tlevel = level;
    // TODO:not confirm how to rename
    for (int i = 0; i < founddatainfo.foundlevelnum; i++)
    {
        tlevel = tlevel->nextlevel;
    }
    std::fstream file(storepos + "/level-" + std::to_string(founddatainfo.foundlevelnum) + "/" + tlevel->buffer[founddatainfo.foundbuffer]->filename + ".sst");

    // read predata
    uint64_t prenum = PRE_SIZE + tlevel->buffer[founddatainfo.foundbuffer]->num * (sizeof(uint64_t) + sizeof(uint32_t)) + founddatainfo.dataoffset;
    char *prechar = new char[prenum];
    file.read(prechar, prenum);
    delete[] prechar;

    // read result
    std::string resultstring = NOT_FOUND;
    if (founddatainfo.valuelength == 0)
    {
        // last value
        std::ostringstream tmp;
        tmp << file.rdbuf();
        resultstring = tmp.str();
    }
    else
    {
        // not last value
        char *resultchar = new char[founddatainfo.valuelength + 1];
        file.read(resultchar, founddatainfo.valuelength);
        resultchar[founddatainfo.valuelength] = 0;
        resultstring = resultchar;
        delete[] resultchar;
    }

    // close file and return
    file.close();
    if (resultstring == DELETED)
        return NOT_FOUND;
    return resultstring;
}

bool Sstable::Isexited(uint64_t key, struct DataInfo &founddatainfo)
{
    // find in level 0
    if (IsexitedinLevel0(key, founddatainfo.foundbuffer, founddatainfo.dataoffset, founddatainfo.valuelength))
    {
        founddatainfo.foundlevelnum = 0;
        return true;
    }

    // find in other level
    struct SSLevel *nlevel = level->nextlevel;
    while (nlevel)
    {
        if (IsexitedinLevelT(key, nlevel, founddatainfo.foundbuffer, founddatainfo.dataoffset, founddatainfo.valuelength))
        {
            founddatainfo.foundlevelnum = nlevel->levelnum;
            return true;
        }
        nlevel = nlevel->nextlevel;
    }
    return false;
}

bool Sstable::IsexitedinLevel0(uint64_t key, uint64_t &foundbuffer, uint64_t &dataoffset, uint64_t &valuelength)
{
    // find in level 0
    uint64_t resulttimestamp = 0, resultbuffer = 0, resultoffset = 0, resultlength = 0;
    bool found = false;
    for (std::vector<struct SSBuffer *>::iterator iter = level->buffer.begin(); iter != level->buffer.end(); iter++)
    {
        // first skip old timestamp
        if ((*iter)->timestamp <= resulttimestamp)
            continue;

        // second judge in range
        if (key < (*iter)->minkey || key > (*iter)->maxkey)
            continue;

        // third judge in BloomFilter
        if (!BloomFilter::IsexitedinBloomFilter(key, (*iter)->BloomFilter))
            continue;

        // fourth judge in bufferdata
        int left = 0, right = (*iter)->data.size();
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if ((*iter)->data[mid].first == key)
            {
                found = true;
                resulttimestamp = (*iter)->timestamp;
                resultbuffer = iter - level->buffer.begin();
                resultoffset = (*iter)->data[mid].second;
                if ((mid + 1) == (*iter)->data.size())
                {
                    resultlength = 0;
                }
                else
                {
                    resultlength = (*iter)->data[mid + 1].second - (*iter)->data[mid].second;
                }
                break;
            }
            else if ((*iter)->data[mid].first < key)
                left = mid + 1;
            else
                right = mid;
        }
    }

    // final copy
    foundbuffer = resultbuffer;
    dataoffset = resultoffset;
    valuelength = resultlength;
    return found;
}

bool Sstable::IsexitedinLevelT(uint64_t key, struct SSLevel *nlevel, uint64_t &foundbuffer, uint64_t &dataoffset, uint64_t &valuelength)
{
    // find in other level
    for (std::vector<struct SSBuffer *>::iterator iter = nlevel->buffer.begin(); iter != nlevel->buffer.end(); iter++)
    {
        // first judge in range
        if (key < (*iter)->minkey || key > (*iter)->maxkey)
            continue;

        // second judge in BloomFilter
        if (!BloomFilter::IsexitedinBloomFilter(key, (*iter)->BloomFilter))
            continue;

        // third judge in bufferdata
        int left = 0, right = (*iter)->data.size();
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if ((*iter)->data[mid].first == key)
            {
                foundbuffer = iter - nlevel->buffer.begin();
                dataoffset = (*iter)->data[mid].second;
                if ((mid + 1) == (*iter)->data.size())
                {
                    valuelength = 0;
                }
                else
                {
                    valuelength = (*iter)->data[mid + 1].second - (*iter)->data[mid].second;
                }
                return true;
                break;
            }
            else if ((*iter)->data[mid].first < key)
                left = mid + 1;
            else
                right = mid;
        }
    }
    return false;
}

// scan
void Sstable::ScanSstable(uint64_t key1, uint64_t key2, uint64_t skiplisttimestamp, std::list<std::pair<uint64_t, std::string>> &slist, std::list<std::pair<uint64_t, std::string>> &list)
{
    // push back skiplist pairs
    std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> allpairs;
    allpairs.push_back(std::make_pair(skiplisttimestamp, slist));

    // push back levels
    struct SSLevel *tlevel = level;
    while (tlevel != NULL)
    {
        GetScanLevelT(key1, key2, tlevel, allpairs);
        tlevel = tlevel->nextlevel;
    }

    // sort
    Mergesort(true, allpairs, list);
}

void Sstable::GetScanLevelT(uint64_t key1, uint64_t key2, struct SSLevel *nlevel, std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs)
{
    // scan every buffer
    for (std::vector<struct SSBuffer *>::iterator iter = nlevel->buffer.begin(); iter != nlevel->buffer.end(); iter++)
    {
        std::list<std::pair<uint64_t, struct DataInfo>> bufferdatalist;
        GetScanBuffer(key1, key2, nlevel->levelnum, iter - nlevel->buffer.begin(), (*iter), bufferdatalist);

        // judge blank and replace string
        if (bufferdatalist.size() == 0)
            continue;
        std::list<std::pair<uint64_t, std::string>> sstabledatalist;
        Replace(bufferdatalist, sstabledatalist);

        // push back data
        allpairs.push_back(std::make_pair((*iter)->timestamp, sstabledatalist));
    }
}

void Sstable::GetScanBuffer(uint64_t key1, uint64_t key2, uint64_t levelnum, uint64_t buffernum, struct SSBuffer *buffer, std::list<std::pair<uint64_t, struct DataInfo>> &bufferdatalist)
{
    // replace and judge key
    if (key1 > key2)
        return;
    if (key1 < buffer->minkey)
        key1 = buffer->minkey;
    if (key2 > buffer->maxkey)
        key2 = buffer->maxkey;

    // get data from buffer
    for (std::vector<std::pair<uint64_t, uint32_t>>::iterator iter = buffer->data.begin(); iter != buffer->data.end() && (*iter).first <= key2; iter++)
    {
        if ((*iter).first >= key1)
        {
            struct DataInfo datainfo = {levelnum, buffernum, (*iter).second, 0};
            if ((iter + 1) == buffer->data.end())
            {
                datainfo.valuelength = 0;
            }
            else
            {
                datainfo.valuelength = (*(iter + 1)).second - (*iter).second;
            }
            bufferdatalist.push_back(std::make_pair((*iter).first, datainfo));
        }
    }
}

// compaction
void Sstable::Compaction()
{
    struct SSLevel *tlevel = level;
    // need to compact
    while (tlevel->sstablenum > MAXCONTAIN(tlevel->levelnum))
    {
        CompactionLevel(tlevel);
        tlevel = tlevel->nextlevel;
    }
    return;
}

void Sstable::CompactionLevel(struct SSLevel *nlevel)
{
    // prepare
    std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> allpairs;

    // original level
    //  get bufferlist from original level
    uint64_t originalminkey = UINT64_MAX, originalmaxkey = 0;
    std::vector<uint64_t> originalbufferlist;
    GetCompactionBuffer(false, nlevel, originalminkey, originalmaxkey, originalbufferlist);

    // get all original level data from file to memory
    GetCompactionAllData(allpairs, originalbufferlist, nlevel);

    // delete original level buffer and sstable
    DeleteSelectedBufferandSstable(nlevel, originalbufferlist);

    // next level
    std::vector<uint64_t> nextbufferlist;
    if (nlevel->nextlevel)
    {
        // get bufferlist from next level
        GetCompactionBuffer(true, nlevel->nextlevel, originalminkey, originalmaxkey, nextbufferlist);

        // get all next level data from file to memory
        GetCompactionAllData(allpairs, nextbufferlist, nlevel->nextlevel);

        // delete next level buffer and sstable
        DeleteSelectedBufferandSstable(nlevel->nextlevel, nextbufferlist);
    }
    else
    {
        // create level
        uint64_t newlevelnum = nlevel->levelnum + 1;
        struct SSLevel *newlevel = new Sstable::SSLevel(newlevelnum);
        nlevel->nextlevel = newlevel;
        leveltotnum++;

        // create dir
        std::string path = storepos + "/level-" + std::to_string(newlevelnum) + "/";
        utils::mkdir(path.c_str());
    }

    // get maxtimestamp
    uint64_t maxtimestamp = 0;
    for (std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>>::iterator iter = allpairs.begin(); iter != allpairs.end(); iter++)
    {
        if ((*iter).first > maxtimestamp)
            maxtimestamp = (*iter).first;
    }

    // sort
    std::list<std::pair<uint64_t, std::string>> list;
    if (nlevel->nextlevel->levelnum + 1 == leveltotnum)
    {
        // last level
        Mergesort(true, allpairs, list);
    }
    else
    {
        // not last level
        Mergesort(false, allpairs, list);
    }

    // build new sstable and buffer
    uint64_t newsstablesize = PRE_SIZE;
    std::list<std::pair<uint64_t, std::string>> newsstablelist;
    uint64_t newsstablekeynum = 0, newsstableminkey = list.front().first, newsstablemaxkey = list.front().first, newsstablefilenum = 0;

    // get old maxfilenum
    for (std::vector<struct SSBuffer *>::iterator iter = nlevel->nextlevel->buffer.begin(); iter != nlevel->nextlevel->buffer.end(); iter++)
    {
        if ((*iter)->timestamp == maxtimestamp)
        {
            int tmp = atoi((*iter)->filename.substr(std::to_string((*iter)->timestamp).length() + 1, (*iter)->filename.length()).c_str());
            newsstablefilenum = newsstablefilenum > tmp ? newsstablefilenum : tmp;
        }
    }
    newsstablefilenum++;

    // write new sstable and buffer
    for (std::list<std::pair<uint64_t, std::string>>::iterator iter = list.begin(); iter != list.end(); iter++)
    {
        newsstablesize += (*iter).second.length() + sizeof(uint64_t) + sizeof(uint32_t);
        if (newsstablesize > MAX_SIZE)
        {
            // build sstable
            BuildSstable(maxtimestamp, newsstablekeynum, newsstableminkey, newsstablemaxkey, newsstablelist, std::to_string(maxtimestamp) + "-" + std::to_string(newsstablefilenum), nlevel->nextlevel);

            // reset parameter
            newsstablesize = PRE_SIZE;
            newsstablelist.clear();
            newsstablekeynum = 0;
            newsstableminkey = (*iter).first;
            newsstablefilenum++;
        }
        newsstablelist.push_back((*iter));
        newsstablekeynum++;
        newsstablemaxkey = (*iter).first;
    }

    // final build last elements into sstable
    BuildSstable(maxtimestamp, newsstablekeynum, newsstableminkey, newsstablemaxkey, newsstablelist, std::to_string(maxtimestamp) + "-" + std::to_string(newsstablefilenum), nlevel->nextlevel);
}

void Sstable::GetCompactionBuffer(bool isnextlevel, struct SSLevel *nlevel, uint64_t &minkey, uint64_t &maxkey, std::vector<uint64_t> &bufferlist)
{
    if (!isnextlevel)
    {
        // level 0
        if (nlevel->levelnum == 0)
        {
            // get all buffer
            for (uint64_t i = 0; i < nlevel->sstablenum; i++)
            {
                bufferlist.push_back(i);

                // change minkey
                if (nlevel->buffer[i]->minkey < minkey)
                    minkey = nlevel->buffer[i]->minkey;

                // change maxkey
                if (nlevel->buffer[i]->maxkey > maxkey)
                    maxkey = nlevel->buffer[i]->maxkey;
            }
        }
        else
        // level T
        {
            // get the first several buffer
            for (uint64_t i = 0; i < nlevel->sstablenum - MAXCONTAIN(nlevel->levelnum); i++)
            {
                bufferlist.push_back(i);

                // change minkey
                if (nlevel->buffer[i]->minkey < minkey)
                    minkey = nlevel->buffer[i]->minkey;

                // change maxkey
                if (nlevel->buffer[i]->maxkey > maxkey)
                    maxkey = nlevel->buffer[i]->maxkey;
            }
        }
    }
    else
    {
        // level T
        for (std::vector<struct SSBuffer *>::iterator iter = nlevel->buffer.begin(); iter != nlevel->buffer.end(); iter++)
        {
            // get overlap buffer
            if ((*iter)->maxkey >= minkey && (*iter)->minkey <= maxkey)
            {
                bufferlist.push_back(iter - nlevel->buffer.begin());
            }
        }
    }
}

void Sstable::GetCompactionAllData(std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs, std::vector<uint64_t> &bufferlist, struct SSLevel *nlevel)
{
    // scan all element in bufferlist
    for (std::vector<uint64_t>::iterator iter = bufferlist.begin(); iter != bufferlist.end(); iter++)
    {
        // get buffer
        struct SSBuffer *nowbuffer = nlevel->buffer[(*iter)];

        // get bufferdatalist
        std::list<std::pair<uint64_t, struct DataInfo>> bufferdatalist;
        GetScanBuffer(nowbuffer->minkey, nowbuffer->maxkey, nlevel->levelnum, (*iter), nowbuffer, bufferdatalist);

        // replace string
        std::list<std::pair<uint64_t, std::string>> sstabledatalist;
        Replace(bufferdatalist, sstabledatalist);

        // push back data
        allpairs.push_back(std::make_pair(nowbuffer->timestamp, sstabledatalist));
    }
}

// delete
void Sstable::DeleteSelectedBufferandSstable(struct SSLevel *nlevel, std::vector<uint64_t> &selectedbufferlist)
{
    // delete sstable files
    for (std::vector<uint64_t>::iterator iter = selectedbufferlist.begin(); iter != selectedbufferlist.end(); iter++)
    {
        utils::rmfile((storepos + "/level-" + std::to_string(nlevel->levelnum) + "/" + nlevel->buffer[(*iter)]->filename + ".sst").c_str());
    }

    // delete buffer
    std::vector<struct SSBuffer *>::iterator beginiter = nlevel->buffer.begin();
    for (int i = 0; i < selectedbufferlist.size(); i++)
    {
        delete[] nlevel->buffer[selectedbufferlist[i] - i]->BloomFilter;
        delete nlevel->buffer[selectedbufferlist[i] - i];
        nlevel->buffer.erase(beginiter + selectedbufferlist[i] - i);
        nlevel->sstablenum--;
    }
}

void Sstable::DeleteAllSstable()
{
    // scan all levels
    for (uint64_t i = 0; i < leveltotnum; i++)
    {
        // get all filenames in a level
        std::vector<std::string> filenames;
        utils::scanDir(storepos + "/level-" + std::to_string(i) + "/", filenames);

        // remove all files
        for (std::vector<std::string>::iterator iter = filenames.begin(); iter != filenames.end(); iter++)
            utils::rmfile((storepos + "/level-" + std::to_string(i) + "/" + (*iter)).c_str());

        // remove dir
        utils::rmdir((storepos + "/level-" + std::to_string(i) + "/").c_str());
    }
}

void Sstable::DeleteAllBuffer()
{
    struct SSLevel *tlevel = level;
    while (tlevel)
    {
        for (std::vector<struct SSBuffer *>::iterator iter = tlevel->buffer.begin(); iter != tlevel->buffer.end(); iter++)
        {
            delete[](*iter)->BloomFilter;
            delete *iter;
        }
        struct SSLevel *plevel = tlevel;
        tlevel = tlevel->nextlevel;
        delete plevel;
    }
}

// reset
void Sstable::Reset()
{
    DeleteAllSstable();
    DeleteAllBuffer();
    leveltotnum = 0;
    level = new Sstable::SSLevel(0);
}

// replace
void Sstable::Replace(std::list<std::pair<uint64_t, struct DataInfo>> &bufferdatalist, std::list<std::pair<uint64_t, std::string>> &sstabledatalist)
{
    // read from file
    struct SSLevel *tlevel = level;
    uint64_t levelnum = bufferdatalist.front().second.foundlevelnum;
    uint64_t buffernum = bufferdatalist.front().second.foundbuffer;
    // TODO:not confirm how to rename
    for (int i = 0; i < levelnum; i++)
    {
        tlevel = tlevel->nextlevel;
    }
    std::fstream file(storepos + "/level-" + std::to_string(levelnum) + "/" + tlevel->buffer[buffernum]->filename + ".sst");

    // read predata
    uint64_t prenum = PRE_SIZE + tlevel->buffer[buffernum]->num * (sizeof(uint64_t) + sizeof(uint32_t));
    char *prechar = new char[prenum];
    file.read(prechar, prenum);
    delete[] prechar;

    // get data
    uint64_t paddr = 0;
    for (std::list<std::pair<uint64_t, struct DataInfo>>::iterator iter = bufferdatalist.begin(); iter != bufferdatalist.end(); iter++)
    {
        // get predata
        uint64_t offset = (*iter).second.dataoffset;
        uint64_t length = (*iter).second.valuelength;
        char *prechar = new char[offset - paddr];
        file.read(prechar, offset - paddr);
        delete[] prechar;

        // get data
        std::string stringdata = NOT_FOUND;
        if (length == 0)
        {
            // last value
            std::ostringstream tmp;
            tmp << file.rdbuf();
            stringdata = tmp.str();
        }
        else
        {
            char *chardata = new char[length + 1];
            file.read(chardata, length);
            chardata[length] = 0;
            stringdata = chardata;
            delete[] chardata;
        }
        paddr += length;

        // store
        sstabledatalist.push_back(std::make_pair((*iter).first, stringdata));
    }
    file.close();
}

// sort
void Sstable::Mergesort(bool isScanorLastlevel, std::vector<std::pair<uint64_t, std::list<std::pair<uint64_t, std::string>>>> &allpairs, std::list<std::pair<uint64_t, std::string>> &list)
{
    bool isempty = true;
    do
    {
        // get minkey
        isempty = true;
        std::vector<uint64_t> deldeque;
        uint64_t minkey = UINT64_MAX;
        uint64_t maxtimestamp = 0;
        std::string minkeystring;
        for (uint64_t i = 0; i < allpairs.size(); i++)
        {
            if (allpairs[i].second.size() == 0)
                continue;
            isempty = false;
            if (allpairs[i].second.front().first == minkey)
            {
                deldeque.push_back(i);
                if (allpairs[i].first > maxtimestamp)
                {
                    minkeystring = allpairs[i].second.front().second;
                    maxtimestamp = allpairs[i].first;
                }
                continue;
            }
            if (allpairs[i].second.front().first < minkey)
            {
                while (deldeque.size())
                    deldeque.pop_back();
                deldeque.push_back(i);
                minkey = allpairs[i].second.front().first;
                minkeystring = allpairs[i].second.front().second;
                maxtimestamp = allpairs[i].first;
            }
        }

        // del minkey
        while (deldeque.size())
        {
            uint64_t numofdellist = deldeque.back();
            deldeque.pop_back();
            allpairs[numofdellist].second.erase(allpairs[numofdellist].second.begin());
        }

        // add key and string pair
        if (isScanorLastlevel && minkeystring == DELETED)
            continue;
        if (!isempty)
            list.push_back(std::make_pair(minkey, minkeystring));
    } while (!isempty);
}