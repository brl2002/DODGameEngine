#include "ThreadPool.h"

ThreadPool::ThreadPool()
	: m_ShouldStop(false)
{
	int numThread = std::thread::hardware_concurrency();

	for (int i = 0; i < numThread; ++i)
	{
		m_ThreadPool.push_back(std::thread(&ThreadPool::ThreadLoop, this));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		m_ShouldStop = true;
	}

	m_LockCondition.notify_all();

	for (std::thread &thread : m_ThreadPool)
	{
		thread.join();
	}
}

void ThreadPool::ThreadLoop()
{
	while (!m_ShouldStop)
	{
		Job job;

		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);

			m_LockCondition.wait(lock, [&]{ return m_ShouldStop || !m_JobQueue.empty(); });

			if (m_ShouldStop && m_JobQueue.empty()) return;

			job = m_JobQueue.front();

			m_JobQueue.pop();
		}

		AddJobCount();

		job();

		SubtractJobCount();
	}
}

void ThreadPool::AddJob(Job job)
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		m_JobQueue.push(job);
	}

	m_LockCondition.notify_one();
}

void ThreadPool::Stop()
{
	m_ShouldStop = true;

	m_LockCondition.notify_all();
}

void ThreadPool::Wait()
{
	while (m_JobCount > 0) {}
}

void ThreadPool::AddJobCount()
{
	std::unique_lock<std::mutex> lock(m_CountMutex);

	++m_JobCount;
}

void ThreadPool::SubtractJobCount()
{
	std::unique_lock<std::mutex> lock(m_CountMutex);

	--m_JobCount;
}