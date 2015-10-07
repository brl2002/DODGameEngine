#pragma once

// Class responsible for managing and processsing any relevant
// resources for the engine and its components.
class ResourceManager
{
	char* m_MapBuffer;

	int m_Width;

	int m_Height;

	int m_TotalMapBufferLength;

	bool bMapIsSet;

	ResourceManager();
	virtual ~ResourceManager();

	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	// Reads a map text file then returns a bool whether the read was successful or not,
	// and whether the specified map file is valid or not.
	bool ReadMapFile(char* fileName);

	// Allocates map buffer array on the heap and returns a pointer to the array.
	// A successful call to ReadMapFile should be called first before calling this,
	// otherwise the function will return a null pointer.
	// ************** Users are responsible for deallocating the array. *****************
	char* AllocateMapBuffer();

	inline const char* GetMapBuffer() { return m_MapBuffer; }

	inline int GetMapBufferWidth() { return m_Width; }
	inline int GetMapBufferHeight() { return m_Height; }
};