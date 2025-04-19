/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "dc/dc.h"
#include "script/script-module.h"
#include "stringid/stringid-cache.h"
#include "script/script-manager-eval.h"
#include "stringid/sidbase.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//StringIdCache* g_pStringIdCache = nullptr; //disabled for now
uintptr_t g_moduleBase = 0;
DC::StackFrame* g_stackFrame = nullptr;
FILE* g_outHandle = nullptr;


int main(int argc, const char* aArgv[])
{
	if (argc > 1)
	{
		Module* pModule = new Module(aArgv[1]);
		if (pModule)
		{
			if (pModule->Login())
			{
				void* pFile = pModule->GetLoadedFilePtr();
				g_moduleBase = reinterpret_cast<uintptr_t>(pFile);
				DC_Header* pHdr = reinterpret_cast<DC_Header*>(pFile);
				//g_pStringIdCache = new StringIdCache();
				g_stackFrame = (DC::StackFrame*)malloc(sizeof(DC::StackFrame) * 2);
				assert(g_stackFrame != NULL);
				//g_pStringIdCache->GetDebugInfo(pHdr);
				int32_t numEntries = pHdr->m_numEntries;
				Entry* pTable = pHdr->m_pStartOfData;
				char buf[256];
				snprintf(buf, 256, "%s.txt", aArgv[1]);
				fopen_s(&g_outHandle, buf, "w");
				if (g_outHandle)
				{
					InitSidbase();
					fprintf(g_outHandle, "-------------[ t2r script disassembler by icemesh ]--------------\n");
					for (int32_t i = 0; i < numEntries; i++)
					{
						pModule->DumpEntry(&pTable[i]);
					}
				}
				if (g_outHandle)
				{
					fclose(g_outHandle);
				}
				ShutdownSidbase();

				free(g_stackFrame);
				puts("Done....");
			}
			delete pModule;
			return 0;
		}
		else
		{
			fprintf(g_outHandle, "Failed to allocate module class\n");
			return -1;
		}
	}
	else
	{
		//print usage
		fprintf(g_outHandle, "Usage: .\\%s ss-xxxxx.bin\n", aArgv[0]);
		return 0;
	}
}
