#include "Common.h"

LARGE_INTEGER DevicePerformanceProfile::Device_Frequency = { 0 };
bool DevicePerformanceProfile::DeviceFrequencyQuery = QueryPerformanceFrequency(&DevicePerformanceProfile::Device_Frequency);

int ArrayAccessHelper::m_AddFactorTable[9] =
{
	-10,	-9,		-8,
	-1,		0,		1,
	8,		9,		10
};

int ArrayAccessHelper::m_CostTable[9] =
{
	1,		1,		1,
	1,		0,		1,
	1,		1,		1
};

int ArrayAccessHelper::m_TotalWidth = 9;
int ArrayAccessHelper::m_TotalHeight = 9;
int ArrayAccessHelper::m_SegmentWidthPerChunk = 3;
int ArrayAccessHelper::m_SegmentHeightPerChunk = 3;
int ArrayAccessHelper::m_ChunkWidth = ArrayAccessHelper::m_TotalWidth / ArrayAccessHelper::m_SegmentWidthPerChunk;

int ArrayAccessHelper::m_NumSegmentPerChunk = 
	ArrayAccessHelper::m_SegmentWidthPerChunk * ArrayAccessHelper::m_SegmentHeightPerChunk;