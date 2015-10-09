#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef std::function<void()> Job;

class ThreadPool
{
	std::vector<std::thread> m_ThreadPool;

	int m_JobCount;

	std::queue<Job> m_JobQueue;

	std::mutex m_QueueMutex;

	std::mutex m_CountMutex;

	std::condition_variable m_LockCondition;

	bool m_ShouldStop;

public:
	ThreadPool();
	virtual ~ThreadPool();

	void Stop();

	inline int GetJobCount() { return m_JobCount; }

	void AddJob(Job job);

	void Wait();

private:
	void ThreadLoop();

	void AddJobCount();

	void SubtractJobCount();
};