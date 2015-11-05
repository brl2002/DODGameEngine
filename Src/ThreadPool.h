#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

template<class DataType>
// Class that describes a job that a worker thread is going to work on,
// which include pointer to a function to be called and array of data and
// length of the array to iterate through.
class JobDesc
{
	//typedef void(*Func)(void*, int);

	void* m_ObjectInst;
	void(*m_Func)(void*, DataType**, int, int, double);
	DataType* m_DataSet;
	int m_StartIndex;
	int m_Length;
	double m_DeltaTime;

public:
	JobDesc() = default;

	JobDesc( void* objectInst, void(*func)(void*, DataType**, int, int, double), DataType** dataSet, int startIndex, int length, double deltaTime )
		:	m_ObjectInst(objectInst),
			m_Func(func),
			m_DataSet(dataSet),
			m_StartIndex(startIndex),
			m_Length(length),
			m_DeltaTime(deltaTime)
	{}

	// Calling this function will run the function based on the description provided.
	void Execute()
	{
		(*m_Func)(m_ObjectInst, m_DataSet, m_StartIndex, m_Length, m_DeltaTime);
	}
};

template<class DataType>
// Pool of threads that works on any jobs that are queued. This thread pool
// is meant to be used for any functions that computes on an array of data.
class ThreadPool
{
	std::vector<std::thread> m_ThreadPool;

	int m_JobCount;

	std::queue<JobDesc<DataType>> m_JobQueue;

	std::mutex m_QueueMutex;

	std::mutex m_CountMutex;

	std::condition_variable m_LockCondition;

	bool m_ShouldStop;

public:
	ThreadPool()
		: m_ShouldStop(false)
	{
		int numThread = std::thread::hardware_concurrency();

		for (int i = 0; i < numThread; ++i)
		{
			m_ThreadPool.push_back(std::thread(&ThreadPool::ThreadLoop, this));
		}
	}

	virtual ~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);

			m_ShouldStop = true;

			m_JobCount = 0;
		}

		m_LockCondition.notify_all();

		for (std::thread &thread : m_ThreadPool)
			thread.join();
	}

	void Stop()
	{
		m_ShouldStop = true;

		m_LockCondition.notify_all();
	}

	inline int GetJobCount() { return m_JobCount; }

	// Add new job to be processed by a thread in the ThreadPool
	void AddNewJob(JobDesc<DataType>&& job)
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);

			m_JobQueue.push(job);

			++m_JobCount;
		}

		m_LockCondition.notify_one();
	}

	void Wait()
	{
		while (m_JobCount > 0) {}
	}

private:
	void ThreadLoop()
	{
		while (!m_ShouldStop)
		{
			JobDesc<DataType> job;

			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				m_LockCondition.wait(lock, [&]{ return m_ShouldStop || !m_JobQueue.empty(); });

				if (m_ShouldStop && m_JobQueue.empty()) return;

				job = std::move(m_JobQueue.front());

				m_JobQueue.pop();
			}

			job.Execute();

			SubtractJobCount();
		}
	}

	void SubtractJobCount()
	{
		std::unique_lock<std::mutex> lock(m_CountMutex);

		--m_JobCount;
	}
};