/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "stringid-cache.h"
#include "../dc/dc.h"
#include "../dc/state-script/state-script.h"
#include <stdio.h>
#include <string.h>

extern FILE* g_outHandle;

StringIdCache::StringIdCache()
{
	m_fileName = nullptr;
	m_numStrings = 0;
	memset(m_array, 0, kMaxStringIdCacheEntries);
	AddScriptConstants();
}

StringIdCache::StringIdCache(const char* fileName)
{
	m_fileName = fileName;
	m_numStrings = 0;
	AddScriptConstants();
}

void StringIdCache::GetDebugInfo(DC_Header* pHdr)
{
	int numEntries = pHdr->m_numEntries;
	if (numEntries)
	{
	//	fprintf(g_outHandle, "Num entries: %d\n", numEntries);
		Entry* pEntry = pHdr->m_pStartOfData;
		for (int k = 0; k < numEntries; k++)
		{
			if (pEntry[k].m_typeId == SID("state-script"))
			{
				StateScript* pSs = reinterpret_cast<StateScript*>(pEntry[k].m_entryPtr);
				//AddStringToLocalSidCache(pSs->m_name);
				SsState* pStateArray = pSs->m_pSsStateTable;
				int16_t numStates = pSs->m_numStates;
				int64_t numSsOnBlocks = 0;
				SsOnBlock* pOnBlock = nullptr;
				char stateName[256];
				for (int i = 0; i < numStates; i++)
				{
					numSsOnBlocks = pStateArray[i].m_numSsOnBlocks;
					if (numSsOnBlocks > 0)
					{
						pOnBlock = pStateArray[i].m_pSsOnBlocks;
						for (int j = 0; j < numSsOnBlocks; j++)
						{
							//sscanf_s(pOnBlock[j].m_debugBlockInfo, "%s %s", arr, 0x10, buf, 256);
							//find the first occurance of space: give that the format of debug info is as follows
							//<name_of_the_script> <state_name> (on ((<start>||<update>||<event>))) we can resolve some StringIds this way..
							const char* ptr = strstr(pOnBlock[j].m_name, " ");
							if (ptr)
							{
								ptr++;
								sscanf_s(ptr, "%s", &stateName, 256);
								//add the state name to our local cache
								AddStringToLocalSidCache(stateName);
							}
							else
							{
								fprintf(g_outHandle, pOnBlock[j].m_name);
							}
						}
					}
				}
			}
		}
	}
}

bool StringIdCache::AddStringToLocalSidCache(const char* pString)
{
	if (pString)
	{
		StringId64 sid = SID(pString);
		//check if the string is present
		int idx = 0;
		for (idx; idx < m_numStrings; idx++)
		{
			if (m_array[idx].m_key == sid)
			{
				return false; //already present
			}
		}
		if (idx < kMaxStringIdCacheEntries)
		{
			/*
#ifdef _DEBUG
			fprintf(g_outHandle, "Adding %s(#%.16llX) to local sid cache\n", pString, sid);
#endif // DEBUG
*/
			m_array[idx].m_key = sid;
			memcpy_s(m_array[idx].m_data, 256, pString, 256);
			m_numStrings++;
		}
		else
		{
			fprintf(g_outHandle, "Increase kMaxStringIdCacheEntries\n");
		}

		return true;
	}
	return false; //invalid string
}

char* StringIdCache::LookupStringId(StringId64 sid)
{
	for (int idx = 0; idx < m_numStrings; idx++)
	{
		if (m_array[idx].m_key == sid)
		{
			return m_array[idx].m_data;
		}
	}
	return nullptr;
}

void StringIdCache::PrintCacheEntries()
{
	FILE* fh;
	fopen_s(&fh, "state-sids.txt", "a");
	if (fh)
	{
		fprintf(g_outHandle, "Please give the state-sids.txt to icemesh once its full");
		for (int i = 0; i < m_numStrings; i++)
		{
			fprintf(fh, "%s\n", m_array[i].m_data);
		}
		fclose(fh);
	}
	else
	{
		fprintf(g_outHandle, "Please add the following strings to the sidbase\n");
		for (int i = 0; i < m_numStrings; i++)
		{
			fprintf(g_outHandle, "%s\n", m_array[i].m_data);
		}
	}
}

void StringIdCache::AddScriptConstants()
{
	/*
	AddStringToLocalSidCache("player");
	AddStringToLocalSidCache("dina");
	AddStringToLocalSidCache("spawned");
	AddStringToLocalSidCache("angle");
	AddStringToLocalSidCache("player-speed");
	AddStringToLocalSidCache("distance");
	AddStringToLocalSidCache("allow-hug");
	AddStringToLocalSidCache("--all--");
	AddStringToLocalSidCache("hide");
	AddStringToLocalSidCache("show");
	AddStringToLocalSidCache("abort");
	AddStringToLocalSidCache("complete");
	AddStringToLocalSidCache("function");
	AddStringToLocalSidCache("state-script");
	AddStringToLocalSidCache("main");
	AddStringToLocalSidCache("array");
	AddStringToLocalSidCache("done");
	AddStringToLocalSidCache("script-lambda");
	AddStringToLocalSidCache("npc-buddy-2");
	AddStringToLocalSidCache("self");
	AddStringToLocalSidCache("wait-spawn-entity");
	AddStringToLocalSidCache("set-symbol");
	AddStringToLocalSidCache("get-symbol");
	AddStringToLocalSidCache("go");
	AddStringToLocalSidCache("get-player-speed");
	AddStringToLocalSidCache("get-float");
	AddStringToLocalSidCache("get-object-locator");
	AddStringToLocalSidCache("get-object-or-spawner-locator");
	AddStringToLocalSidCache("start-igc");
	AddStringToLocalSidCache("signal");
	AddStringToLocalSidCache("#%alloc-array");
	AddStringToLocalSidCache("#%alloc-mem");
	AddStringToLocalSidCache("single-frame");
	AddStringToLocalSidCache("wait");
	AddStringToLocalSidCache("wait-for");
	AddStringToLocalSidCache("npc-set-demeanor");
	AddStringToLocalSidCache("set-float");
	AddStringToLocalSidCache("angle-in-between-entities");
	AddStringToLocalSidCache("npc-set-script-only-no-perception");
	AddStringToLocalSidCache("npc-stop");
	AddStringToLocalSidCache("wait-animate_");
	AddStringToLocalSidCache("task-complete!");
	AddStringToLocalSidCache("wait-one-frame");
	AddStringToLocalSidCache("task-complete?");
	*/

}