#include <iostream>
#include "thread_pool.h"

struct test
{
	test() = default;
	test(const test&) { std::cout << "copy construct" << std::endl; }
	test(test&&) { std::cout << "move construct" << std::endl; }
};

void hello(const test&)
{
	std::cout << "Hello world!" << std::endl;
}

int main()
{
	thread_pool tp(std::thread::hardware_concurrency());
	test t;
	tp.async(&hello, t).wait();
	tp.async(&hello, std::move(t)).wait();
	tp.async(&hello, std::ref(t)).wait();
	tp.async(&hello, std::cref(t)).wait();
	tp.stop();
	return 0;
}
