#include "kvstore.h"

// /*
//  * Here is test for not use skiplist
//  * Below is really function which use skiplist
//  */
// KVStore::KVStore(const std::string &dir) : KVStoreAPI(dir)
// {
// 	// create map
// 	rbmap = new std::map<int,std::string>();

// 	// create sstable
// 	sstable = new Sstable(dir);

// 	// read from sstable files
// 	timestamp = sstable->InitializeSstable() + 1;
// }

// KVStore::~KVStore()
// {
// 	// store current map and delete map
// 	uint64_t num = 0, minkey = 0, maxkey = 0;
// 	std::list<std::pair<uint64_t, std::string>> slist;
// 	std::map<int,std::string>::iterator iter=rbmap->begin();
// 	minkey=(*iter).first;
// 	for (;iter!=rbmap->end();iter++)
// 	{
// 		slist.push_back(std::make_pair((*iter).first,(*iter).second));
// 		maxkey=(*iter).first;
// 		num++;
// 	}
// 	if (slist.size() != 0)
// 	{
// 		sstable->BuildSstableFromSkipList(timestamp, num, minkey, maxkey, slist);
// 	}
// 	delete rbmap;

// 	// delete buffer
// 	sstable->DeleteAllBuffer();

// 	// delete sstable
// 	delete sstable;
// }

// /**
//  * Insert/Update the key-value pair.
//  * No return values for simplicity.
//  */
// void KVStore::put(uint64_t key, const std::string &s)
// {
// 	// check if oversized
// 	if (size+s.length() > MAX_SIZE)
// 	{
// 		// store current map and delete map
// 		uint64_t num = 0, minkey = 0, maxkey = 0;
// 		std::list<std::pair<uint64_t, std::string>> slist;
// 		std::map<int, std::string>::iterator iter = rbmap->begin();
// 		minkey = (*iter).first;
// 		for (; iter != rbmap->end(); iter++)
// 		{
// 			slist.push_back(std::make_pair((*iter).first, (*iter).second));
// 			maxkey=(*iter).first;
// 			num++;
// 		}
// 		if (slist.size() != 0)
// 		{
// 			sstable->BuildSstableFromSkipList(timestamp, num, minkey, maxkey, slist);
// 		}
// 		delete rbmap;

// 		// create new skiplist and set timestamp
// 		rbmap = new std::map<int,std::string>();
// 		timestamp++;
// 		size=PRE_SIZE;
// 	}
// 	rbmap->insert(std::make_pair(key,s));
// 	size+=s.length() + sizeof(uint32_t) + sizeof(uint64_t);
// }
// /**
//  * Returns the (string) value of the given key.
//  * An empty string indicates not found.
//  */
// std::string KVStore::get(uint64_t key)
// {
// 	// get from map
// 	std::map<int,std::string>::iterator iter = rbmap->find(key);
// 	if (iter!=rbmap->end())
// 	{
// 		if ((*iter).second==DELETED)
// 			return NOT_FOUND;
// 		return (*iter).second;
// 	}

// 	// get from sstable
// 	return sstable->SearchSstable(key);
// }
// /**
//  * Delete the given key-value pair if it exists.
//  * Returns false iff the key is not found.
//  */
// bool KVStore::del(uint64_t key)
// {
// 	// search from all data
// 	if (get(key) == NOT_FOUND)
// 		return false;

// 	// insert DELETED
// 	put(key, DELETED);
// 	return true;
// }

// /**
//  * This resets the kvstore. All key-value pairs should be removed,
//  * including memtable and all sstables files.
//  */
// void KVStore::reset()
// {
// }

// /**
//  * Return a list including all the key-value pair between key1 and key2.
//  * keys in the list should be in an ascending order.
//  * An empty string indicates not found.
//  */
// void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string>> &list)
// {
// }
/*
 * Here is really function which use skiplist
 * Above is test for not use skiplist
 */
KVStore::KVStore(const std::string &dir) : KVStoreAPI(dir)
{
	// create skiplist
	skiplist = new SkipList();

	// create sstable
	sstable = new Sstable(dir);

	// read from sstable files
	timestamp = sstable->InitializeSstable() + 1;
}

KVStore::~KVStore()
{
	// store current skiplist and delete skiplist
	uint64_t num = 0, minkey = 0, maxkey = 0;
	skiplist->Getdata(num, minkey, maxkey);
	std::list<std::pair<uint64_t, std::string>> slist;
	skiplist->SecSearch(minkey, maxkey, slist);
	if (slist.size() != 0)
	{
		sstable->BuildSstableFromSkipList(timestamp, num, minkey, maxkey, slist);
	}
	delete skiplist;

	// delete buffer
	sstable->DeleteAllBuffer();

	// delete sstable
	delete sstable;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	// check if oversized
	if (skiplist->Insert(key, s) == OVERSIZE)
	{
		// create sstable and delete skiplist
		uint64_t num = 0, minkey = 0, maxkey = 0;
		skiplist->Getdata(num, minkey, maxkey);
		std::list<std::pair<uint64_t, std::string>> slist;
		skiplist->SecSearch(minkey, maxkey, slist);
		sstable->BuildSstableFromSkipList(timestamp, num, minkey, maxkey, slist);
		delete skiplist;

		// create new skiplist and set timestamp
		skiplist = new SkipList;
		timestamp++;
		if (skiplist->Insert(key, s) != CORRECT)
		{
			printf("Error!");
			exit(0);
		}
	}
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	// get from skiplist
	SKNode *result = skiplist->Search(key);
	if (result != NULL)
	{
		if (result->val == DELETED)
			return NOT_FOUND;
		return result->val;
	}

	// get from sstable
	return sstable->SearchSstable(key);
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	// search from all data
	if (get(key) == NOT_FOUND)
		return false;

	// insert DELETED
	put(key, DELETED);
	return true;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	// reset skiplist
	delete skiplist;
	skiplist = new SkipList;

	// reset sstable
	sstable->Reset();

	// reset timestamp
	timestamp = 1;
}

/**
 * Return a list including all the key-value pair between key1 and key2.
 * keys in the list should be in an ascending order.
 * An empty string indicates not found.
 */
void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string>> &list)
{
	// scan from skiplist
	std::list<std::pair<uint64_t, std::string>> slist;
	skiplist->SecSearch(key1, key2, slist);

	// scan from sstable and merge
	sstable->ScanSstable(key1, key2, timestamp, slist, list);
}