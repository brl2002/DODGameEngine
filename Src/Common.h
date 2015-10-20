/******************************************************************************
*  Example of a chunk in 3x3 dimension, arranged linearly (2D in abstract)
*  . . .
*  . . .
*  . . .
*
*  And this chunks are arranged linearly (2D in abstract)
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  -----------------------------
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  -----------------------------
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  -----------------------------
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  . . . | . . . | . . . | . . .
*  Example of 4x4 array of 3x3 chunks
******************************************************************************/

#pragma once

#include <chrono>

typedef std::chrono::high_resolution_clock EngineClock;

extern bool g_IsRunning;

// ArrayAccessHelper is responsible for helping accessing array betwen simple linear array
// and array that accesses memory in chunks.
class ArrayAccessHelper
{
	static int m_AddFactorTable[9];
	static int m_CostTable[9];

	static int m_NumSegmentPerChunk;
	static int m_ChunkWidth;
	static int m_TotalWidth;
	static int m_TotalHeight;
	static int m_SegmentWidthPerChunk;
	static int m_SegmentHeightPerChunk;

private:
	ArrayAccessHelper();

public:
	static void Setup(int numSegmentPerChunk,
		int chunkWidth,
		int totalWidth,
		int totalHeight,
		int segmentWidthPerChunk,
		int segmentHeightPerChunk)
	{
		m_NumSegmentPerChunk = numSegmentPerChunk;
		m_ChunkWidth = chunkWidth;
		m_TotalWidth = totalWidth;
		m_TotalHeight = totalHeight;
		m_SegmentWidthPerChunk = segmentWidthPerChunk;
		m_SegmentHeightPerChunk = segmentHeightPerChunk;
	}

	inline static int GetAddFactor(int index) { return m_AddFactorTable[index]; }

	inline static int GetCost(int index) { return m_CostTable[index]; }

	// Function returns an column index for accessing a simple linear array for a given index for a chunk-array.
	inline static int GetSimpleColumnIndex(int chunkIndex)
	{
		return ((chunkIndex % m_NumSegmentPerChunk) % m_SegmentWidthPerChunk) +
			((chunkIndex / m_NumSegmentPerChunk) % m_ChunkWidth) * m_SegmentWidthPerChunk;
	}

	// Function returns an row index for accessing a simple linear array for a given index for a chunk-array.
	inline static int GetSimpleRowIndex(int chunkIndex)
	{
		return (chunkIndex / (m_NumSegmentPerChunk * m_ChunkWidth)) *
			m_SegmentWidthPerChunk + (chunkIndex % m_NumSegmentPerChunk) / m_SegmentWidthPerChunk;
	}

	// Function returns final index for accessing a simple linear array for a given index for a chunk-array.
	static int GetSimpleIndex(int chunkIndex)
	{
		return GetSimpleRowIndex(chunkIndex) * 9 + GetSimpleColumnIndex(chunkIndex);
	}

	// Function returns final index for accessing a chunk-bsed array for a given simple linear index.
	static int GetChunkIndex(int simpleIndex)
	{
		int columnIndex = simpleIndex % m_TotalWidth;
		int rowIndex = simpleIndex / m_TotalWidth;
		int chunkColumnIndex = columnIndex / m_SegmentWidthPerChunk;
		int chunkRowIndex = rowIndex / m_SegmentHeightPerChunk;
		int innerSegmentIndex = (columnIndex % m_SegmentWidthPerChunk)
			+ (rowIndex % m_SegmentHeightPerChunk) * m_SegmentWidthPerChunk;

		return (chunkRowIndex * (m_TotalWidth / m_SegmentWidthPerChunk) + chunkColumnIndex)
			* (m_SegmentWidthPerChunk * m_SegmentHeightPerChunk) + innerSegmentIndex;
	}
};