#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include "thread_pool.h"

template <class _it>
void parallel_sort(thread_pool& tp, _it l, _it r)
{
	if (l == r)
	{
		return;
	}

	if (tp.free_size() == 0)
	{
		std::sort(l, r);
		return;
	}

	_it m = [=] ()
	{
		_it pivot = r - 1;
		_it p = l, q = r - 2;
		while (p <= q)
		{
			if (*p < *pivot) ++p;
			else std::swap(*p, *(q--));
		}

		std::swap(*(q + 1), *pivot);
		return q + 1;
	} ();

	auto result = tp.async(&parallel_sort<_it>, std::ref(tp), l, m);
	parallel_sort(tp, m + 1, r);
	result.wait();
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	
	size_t n;
	if (argc == 1)
	{
		std::cin >> n;
	}
	else
	{
		n = atoi(argv[1]);
	}

	std::vector<int> v0;
	v0.reserve(n);
	for (size_t i = 0; i < n; ++i)
	{
		v0.push_back(rand());
	}

	std::vector<int> v1(v0);

	{
		auto t = std::chrono::high_resolution_clock::now();
		std::sort(v0.begin(), v0.end());
		std::cout << "std::sort duration: " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t).count() << std::endl;

		if (n <= 10)
		{
			for (int i: v0) std::cout << i << std::endl;;
		}
	}

	{
		auto t = std::chrono::high_resolution_clock::now();
		thread_pool tp(std::thread::hardware_concurrency());
		parallel_sort(tp, v1.begin(), v1.end());
		tp.stop();
		std::cout << "parallel_sort duration: " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t).count() << std::endl;

		if (n <= 10)
		{
			for (int i: v1) std::cout << i << std::endl;
		}
	}

	return 0;
}
