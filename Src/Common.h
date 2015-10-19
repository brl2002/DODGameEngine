#pragma once

#include <chrono>

typedef std::chrono::high_resolution_clock EngineClock;

extern bool g_IsRunning;

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

// ArrayAccessHelper is responsible for helping accessing array betwen simple linear array and array that accesses
// memory in chunks.
class ArrayAccessHelper
{
public:
	// Function returns an column index for accessing a simple linear array for a given index for a chunk-array.
	inline static int GetSimpleColumnIndex(int chunkIndex, int numSegment, int segWidthPerChunk, int chunkWidth)
	{
		// primary column = (i % numSegment) % segWidthPerChunk
		// secondary column = (i / numSegment) % chunkWidth
		// column = ((i % numSegment) % segWidthPerChunk) + ((i / numSegment) % chunkWidth) * segWidthPerChunk

		return ((chunkIndex % numSegment) % segWidthPerChunk) + ((chunkIndex / numSegment) % chunkWidth) * segWidthPerChunk;
	}

	// Function returns an row index for accessing a simple linear array for a given index for a chunk-array.
	inline static int GetSimpleRowIndex(int chunkIndex, int numSegment, int segWidthPerChunk, int chunkWidth)
	{
		// primary row = (i % numSegment) / segWidthPerChunk
		// secondary row = i / (numSegment * chunkWidth)
		// row = (i / (numSegment * chunkWidth)) * segWidthPerChunk + (i % numSegment) / segWidthPerChunk

		return (chunkIndex / (numSegment * chunkWidth)) * segWidthPerChunk + (chunkIndex % numSegment) / segWidthPerChunk;
	}

	// Function returns final index for accessing a simple linear array for a given index for a chunk-array.
	static int GetSimpleIndex(int chunkIndex, int numSegment, int segWidthPerChunk, int chunkWidth)
	{
		return GetSimpleRowIndex(chunkIndex, numSegment, segWidthPerChunk, chunkWidth) * 9
			+ GetSimpleColumnIndex(chunkIndex, numSegment, segWidthPerChunk, chunkWidth);
	}
};