
LINK.o = $(LINK.cc)
CXXFLAGS = -g -std=c++14 -Wall

all: correctness persistence

correctness: kvstore.o correctness.o Sstable.o SkipList.o BloomFilter.o

persistence: kvstore.o persistence.o Sstable.o SkipList.o BloomFilter.o

performancetest: kvstore.o persistence.o Sstable.o SkipList.o BloomFilter.o

clean:
	-rm -f correctness persistence performancetest *.o
