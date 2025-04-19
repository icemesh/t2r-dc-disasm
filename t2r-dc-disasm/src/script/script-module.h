/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#pragma once
#include <stdint.h>

struct Entry;

class Module
{
public:
	Module();

	Module(const char* filename);

	~Module();

	int32_t Login();

	void DumpEntry(Entry* pEntry);

	int32_t ScriptLogin(uint8_t* pFile, uint8_t* pFile1, uint64_t lowerBound, uint64_t upperBound);

	inline void* GetLoadedFilePtr() { return m_pLoadedFile; }
private:
	void* m_pLoadedFile;
};