#pragma once

#include <string>
#include "kvstore_api.h"
#include "SkipList.h"
#include "Sstable.h"

#define NOT_FOUND ""
#define DELETED "~DELETED~"

class KVStore : public KVStoreAPI
{
	// You can add your implementation here
private:
	uint64_t timestamp = 1;
	SkipList *skiplist;
	Sstable *sstable;

public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

	void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string>> &list) override;
};
