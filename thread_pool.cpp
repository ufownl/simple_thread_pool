#include "thread_pool.h"

thread_pool::thread_pool(size_t sz)
	: m_free_size(sz)
	, m_state(sz ? state::RUNNING : state::INVALID)
{
	m_threads.reserve(sz ? sz : 1);
	for (size_t i = 0; i < sz; ++i)
	{
		m_threads.emplace_back(&thread_pool::thread_func, this);
	}
}

void thread_pool::stop()
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_state = state::STOP;
	}
	m_cv.notify_all();
}

void thread_pool::join()
{
	for (std::thread& t: m_threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

size_t thread_pool::job_size()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_jobs.size();
}

void thread_pool::dispatch(job_type job)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_jobs.push(std::move(job));
	}
	m_cv.notify_one();
}

void thread_pool::thread_func()
{
	for (;;)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this] { return !m_jobs.empty() || m_state != state::RUNNING; } );

		if (m_state != state::RUNNING)
		{
			break;
		}

		job_type job = std::move(m_jobs.front());
		m_jobs.pop();

		lock.unlock();

		--m_free_size;
		job();
		++m_free_size;
	}
}
