/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#pragma once

#include "stringid.h"

struct DC_Header;

struct CachedStringId
{
	StringId64	m_key;
	char		m_data[256];
};

class StringIdCache
{
public:
	StringIdCache();

	StringIdCache(const char* fileName);

	void AddScriptConstants();

	void GetDebugInfo(DC_Header* pHdr);

	bool AddStringToLocalSidCache(const char* pString);

	char* LookupStringId(StringId64 sid);

	void PrintCacheEntries();

	static const int kMaxStringIdCacheEntries = 256;
private:
	const char*			m_fileName;
	int					m_numStrings;
	CachedStringId		m_array[kMaxStringIdCacheEntries];
};

//extern StringIdCache* g_pStringIdCache;
