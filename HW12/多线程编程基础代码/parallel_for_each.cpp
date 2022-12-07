#include <vector>
#include <thread>

template <class Iterator, class Function>
void parallel_for_each_item(Iterator first, Iterator last, Function fn, int N)
{
    while (first < last)
    {
        fn(*first);
        first += N;
    }
}

template <class Iterator, class Function>
Function parallel_for_each(Iterator first, Iterator last, Function fn, int N)
{
    std::vector<std::thread> threads;

    for (int i = 0; i < N; i++)
        threads.emplace_back(parallel_for_each_item, first + i, last, fn, N);

    for (int i = 0; i < N; i++)
        threads[i].join();

    return fn; // or, since C++11: return move(fn);
}
