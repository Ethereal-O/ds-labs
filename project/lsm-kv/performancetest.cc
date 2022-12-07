#include <iostream>
#include <cstdint>
#include <string>
#include <ctime>

#include "test.h"

class PerformanceTest
{
private:
	class KVStore store;
	const uint64_t LARGE_TEST_MAX = 1024 * 64;

	void regular_test(uint64_t max)
	{
		clock_t start, finish1, finish2;
		double Time1, Time2;
		start = clock();
		uint64_t i;

		// Test a single key
		store.get(1);
	}

public:

	PerformanceTest(const std::string &dir):store(dir)
	{
	}

	void start_test(void *args = NULL)
	{
		std::cout << "KVStore Performance Test" << std::endl;

		regular_test(LARGE_TEST_MAX);
	}
};

int main(int argc, char *argv[])
{

	PerformanceTest test("./data");
	test.start_test();

	return 0;
}
