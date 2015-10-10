#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef void(*Func)(void*, int);

// Class that describes a job that a worker thread is going to work on,
// which include pointer to a function to be called and array of data and
// length of the array to iterate through.
class JobDesc
{
	Func m_Func;
	void* m_DataSet;
	int m_Length;

public:
	JobDesc() = default;

	JobDesc(Func func, void* dataSet, int length)
		: m_Func(func), m_DataSet(dataSet), m_Length(length) {}

	// Calling this function will run the function based on the description provided.
	void Execute()
	{
		(*m_Func)(m_DataSet, m_Length);
	}
};

// Pool of threads that works on any jobs that are queued. This thread pool
// is meant to be used for any functions that computes on an array of data.
class ThreadPool
{
	std::vector<std::thread> m_ThreadPool;

	int m_JobCount;

	std::queue<JobDesc> m_JobQueue;

	std::mutex m_QueueMutex;

	std::mutex m_CountMutex;

	std::condition_variable m_LockCondition;

	bool m_ShouldStop;

public:
	ThreadPool();
	virtual ~ThreadPool();

	void Stop();

	inline int GetJobCount() { return m_JobCount; }

	void AddJob(JobDesc&& job);

	void Wait();

private:
	void ThreadLoop();

	void AddJobCount();

	void SubtractJobCount();
};