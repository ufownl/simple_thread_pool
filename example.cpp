#include <iostream>
#include "thread_pool.h"

#define TASK_CNT	10

int fib(int n)
{
	return n < 3 ? 1 : fib(n - 1) + fib(n - 2);
}

int main()
{
	thread_pool tp(std::thread::hardware_concurrency());
	
	std::future<int> results[TASK_CNT];
	for (size_t i = 0; i < TASK_CNT; ++i)
	{
		printf("dispatch job[%lu].\n", i);
		results[i] = tp.async(&fib, 40);
	}

	for (size_t i = 0; i < TASK_CNT; ++i)
	{
		printf("free size: %lu\n", tp.free_size());
		printf("job size: %lu\n", tp.job_size());
		printf("result[%lu]: %d\n", i, results[i].get());
	}

	printf("free size: %lu\n", tp.free_size());
	printf("job size: %lu\n", tp.job_size());

	tp.stop();

	return 0;
}
