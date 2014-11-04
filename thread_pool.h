#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <atomic>
#include <queue>
#include <memory>
#include <functional>
#include <type_traits>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

class thread_pool
{
public:
	explicit thread_pool(size_t sz);
	~thread_pool() noexcept { join(); }

	template <class F, class... ARGS>
	std::future<typename std::result_of<F(ARGS...)>::type> async(F&& func, ARGS&&... args)
	{
		typedef std::packaged_task<typename std::result_of<F(ARGS...)>::type(ARGS...)> task_type;
		
		auto task = std::make_shared<task_type>(std::forward<F>(func));

		dispatch(std::bind([task] (ARGS... args) mutable
					{
						(*task)(std::forward<ARGS>(args)...);
					}, std::forward<ARGS>(args)...));

		return task->get_future();
	}

	void stop();
	void join();

	size_t thread_size() const { return m_threads.size(); }
	size_t free_size() const { return m_free_size.load(); }
	size_t job_size();

public:
	thread_pool(const thread_pool&) = delete;
	thread_pool& operator = (const thread_pool&) = delete;

private:
	typedef std::function<void()> job_type;

	enum class state
	{
		INVALID,
		RUNNING,
		STOP
	};

private:
	void dispatch(job_type job);
	void thread_func();

private:
	std::vector<std::thread> m_threads;
	std::atomic<size_t> m_free_size;

	std::queue<job_type> m_jobs;
	state m_state;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

#endif  // __THREAD_POOL_H__
