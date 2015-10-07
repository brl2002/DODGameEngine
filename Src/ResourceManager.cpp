#include "ResourceManager.h"

#include <fstream>
#include <string>

ResourceManager::ResourceManager()
	: m_MapBuffer(nullptr), m_Width(0), m_Height(0), m_TotalMapBufferLength(0), bMapIsSet(false)
{}

ResourceManager::~ResourceManager()
{
	if (m_MapBuffer != nullptr) delete[] m_MapBuffer;
}

bool ResourceManager::ReadMapFile(char* fileName)
{
	char *dir = "../Resources/";
	int finalNameLength = std::strlen(dir) + std::strlen(fileName) + 1;
	char* finalName = new char[finalNameLength];
	strcpy_s(finalName, finalNameLength, dir);
	strcat_s(finalName, finalNameLength, fileName);

	std::ifstream mapFile;
	mapFile.open(finalName);

	delete[] finalName;

	// If map file is not open then reading should be stopped.
	if (!mapFile.is_open()) return false;

	int lineCount = 0;

	// Check and see if the map file is valid, and also fill out the dimension info.
	while (!mapFile.eof())
	{
		char line[100];

		mapFile >> line;

		int lineLength = strlen(line);

		if (lineCount == 0)
		{
			m_Width = lineLength;
		}

		// If line length is greater than the set width, then the file is not valid and reading should be stopped.
		if (lineLength != m_Width)
		{
			mapFile.close();

			return false;
		}

		++lineCount;
	}

	m_Height = lineCount - 1;

	mapFile.clear();
	mapFile.seekg(0);
	lineCount = 0;

	m_TotalMapBufferLength = (m_Width + 1) * m_Height + 1;

	m_MapBuffer = new char[m_TotalMapBufferLength];

	// Read each line in the file until we reach end of the file.
	while (!mapFile.eof())
	{
		char line[100];

		mapFile >> line;

		int lineLength = strlen(line);

		line[m_Width] = '\n';
		line[m_Width + 1] = '\0';

		if (lineCount == 1)
		{
			strcpy_s(m_MapBuffer, m_TotalMapBufferLength, line);
		}
		else if (lineCount > 1)
		{
			strcat_s(m_MapBuffer, m_TotalMapBufferLength, line);
		}

		++lineCount;
	}

	mapFile.close();

	bMapIsSet = true;

	return bMapIsSet;
}

char* ResourceManager::AllocateMapBuffer()
{
	// If map has not been read from a file then return null pointer.
	if (!bMapIsSet) return nullptr;

	char* newMapBuffer = new char[m_TotalMapBufferLength];

	memcpy(newMapBuffer, m_MapBuffer, m_TotalMapBufferLength);

	return newMapBuffer;
}