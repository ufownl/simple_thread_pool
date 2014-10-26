#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "thread_pool.h"

#define THREAD_CNT	4

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

double now()
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main()
{
	srand(time(nullptr));
	
	size_t n;
	std::cin >> n;

	std::vector<int> v0;
	v0.reserve(n);
	for (size_t i = 0; i < n; ++i)
	{
		v0.push_back(rand());
	}

	std::vector<int> v1(v0);

	{
		double t = now();
		std::sort(v0.begin(), v0.end());
		std::cout << "std::sort duration: " << now() - t << std::endl;

		if (n <= 10)
		{
			for (int i: v0) std::cout << i << std::endl;;
		}
	}

	{
		double t = now();
		thread_pool tp(THREAD_CNT);
		parallel_sort(tp, v1.begin(), v1.end());
		tp.stop();
		std::cout << "parallel_sort duration: " << now() - t << std::endl;

		if (n <= 10)
		{
			for (int i: v1) std::cout << i << std::endl;
		}
	}

	return 0;
}
