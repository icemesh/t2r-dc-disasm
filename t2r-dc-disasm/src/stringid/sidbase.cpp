/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "sidbase.h"
#include "stringid-cache.h"

#include <stdio.h>
#include <stdlib.h>

extern FILE* g_outHandle;

struct SidbaseEntry
{
	StringId64	m_id;
	size_t		m_offset;
};

struct Sidbase
{
	Sidbase()
	{
		m_pFile = nullptr;
		m_pData = nullptr;
		m_pStrings = nullptr;
		m_numEntires = 0;
	}
	uint8_t* m_pFile;
	SidbaseEntry* m_pData;
	const char* m_pStrings;
	uint64_t m_numEntires;
};

Sidbase g_sidbase;

bool g_bSidbaseAvailable = false;
char g_sidBuffer[0x20];


void InitSidbase()
{
	FILE* fh;
	fopen_s(&fh, "sid1/sidbase.bin", "rb");
	if (fh)
	{
		fseek(fh, 0x0, SEEK_END);
		size_t fsize = ftell(fh);
		fseek(fh, 0x0, SEEK_SET);
		uint8_t* pMem = reinterpret_cast<uint8_t*>(malloc(fsize));
		if (!pMem)
		{
			fprintf(g_outHandle, "Failed to allocate 0x%010llX bytes for sid db\n", fsize);
			fclose(fh);
		}
		else
		{
			fread(pMem, fsize, 0x1, fh);
			fclose(fh);
			g_sidbase.m_pFile = pMem;
			g_sidbase.m_pData = reinterpret_cast<SidbaseEntry*>(pMem + 0x8);
			g_sidbase.m_numEntires = *reinterpret_cast<uint64_t*>(pMem);
			g_bSidbaseAvailable = true;
		}
	}
	else
	{
		fprintf(g_outHandle, "Could not open sid1/sidbase.bin!- Does it even exist ?\n");
	}
}

void ShutdownSidbase()
{
	if (g_sidbase.m_pFile != NULL)
	{
		free(g_sidbase.m_pFile);
	}
}

char* StringIdToStringInternal(uint64_t sid)
{
	if (g_bSidbaseAvailable)
	{
		//linear search -- need to improve
		uint64_t nEntries = g_sidbase.m_numEntires;
		SidbaseEntry* pEntry = g_sidbase.m_pData;
		for (uint64_t iEntries = 0; iEntries < nEntries; iEntries++)
		{
			if (pEntry->m_id == sid)
			{
				return reinterpret_cast<char*>(g_sidbase.m_pFile) + pEntry->m_offset;
			}
			pEntry++;
		}
	}
	//else lookup in the local cache
	/*
	if (g_pStringIdCache != nullptr)
	{
		char* pStr = g_pStringIdCache->LookupStringId(sid);
		if (pStr)
		{
			return pStr;
		}
	}
	*/
	//if nothing else worked just print the sid
	snprintf(g_sidBuffer, 0x20, "#%.16llX", sid);
	return g_sidBuffer;
}

