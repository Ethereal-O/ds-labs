#include <iostream>
#include <cstdint>
#include <string>
#include <ctime>
#include <fstream>

#include "kvstore.h"

class PerformanceTest
{
private:
	class KVStore store;
	const uint64_t LARGE_TEST_MAX = 1024 * 64;

	void regular_test(uint64_t max)
	{
		clock_t start, finish;
		double time;
		uint64_t i;

		store.reset();

		// // PART ONE
		// // test put time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.put(i, std::string(i + 1, 's'));
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("pust test total time: %lf\n", time);
		// printf("pust test num per time: %lf\n", max/time);
		// printf("pust test time per num: %lf\n", time/max);

		// // test get time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.get(i);
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("get test total time: %lf\n", time);
		// printf("get test num per time: %lf\n", max/time);
		// printf("get test time per num: %lf\n", time/max);

		// // test del time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.del(i);
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("del test total time: %lf\n", time);
		// printf("del test num per time: %lf\n", max/time);
		// printf("del test time per num: %lf\n", time/max);

		// // PART TWO
		// // put
		// for (i = 0; i < max/2; ++i)
		// {
		// 	store.put(i, std::string(i + 1, 's'));
		// }

		// // test get time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.get(i);
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("get test total time: %lf\n", time);
		// printf("get test time per num: %lf\n", time/max);

		// PART THREE
		// put
		std::list<int> numlist;
		int nownum=0;
		start = clock();
		for (i = 0; i < max; ++i)
		{
			store.put(i, std::string(i + 1, 's'));
			finish=clock();
			if((finish - start) / CLOCKS_PER_SEC>1)
			{
				numlist.push_back(i-nownum);
				nownum=i;
				start=finish;
			}
		}
		std::ofstream file("performance_part3_result.txt");
		for (std::list<int>::iterator iter=numlist.begin();iter!=numlist.end();iter++)
		{
			file<<(*iter)<<std::endl;
		}

		// // PART Four
		// // test put time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.put(i, std::string(i + 1, 's'));
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("pust test total time: %lf\n", time);
		// printf("pust test num per time: %lf\n", max/time);
		// printf("pust test time per num: %lf\n", time/max);

		// // test get time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.get(i);
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("get test total time: %lf\n", time);
		// printf("get test num per time: %lf\n", max/time);
		// printf("get test time per num: %lf\n", time/max);

		// // test del time
		// start = clock();
		// for (i = 0; i < max; ++i)
		// {
		// 	store.del(i);
		// }
		// finish = clock();
		// time = (double)(finish - start) / CLOCKS_PER_SEC;
		// printf("del test total time: %lf\n", time);
		// printf("del test num per time: %lf\n", max/time);
		// printf("del test time per num: %lf\n", time/max);


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

