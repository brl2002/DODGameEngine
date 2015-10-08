#pragma once

#include <chrono>
#include "ResourceManager.h"

typedef std::chrono::high_resolution_clock EngineClock;

extern bool g_IsRunning;

class WorkDesc
{
	void* m_DataSet;
	int m_Length;

public:
	WorkDesc(void* dataSet, int length)
		: m_DataSet(dataSet), m_Length(length) {}

	inline void* GetDataSet() { return m_DataSet; }
	inline int GetLength() { return m_Length; }
};