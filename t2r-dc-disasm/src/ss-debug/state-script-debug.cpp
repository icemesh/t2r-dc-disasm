/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "state-script-debug.h"
#include "../dc/state-script/state-script.h"
#include "../script/script-module.h"
#include "../script/script-manager-eval.h"
#include "../stringid/sidbase.h"


#include <stdio.h>

extern uintptr_t g_moduleBase;
extern FILE* g_outHandle;

void PrintVariable(SsDeclaration* pDecl)
{
	switch (pDecl->m_declTypeId)
	{
		case SID("boolean"):
		{
			uint32_t* pVal = reinterpret_cast<uint32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var boolean '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal == 1 ? "#t" : "#f", ((uintptr_t)pVal-g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var boolean '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("vector"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var vector '%s = %.4f %.4f %.4f %.4f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], pVal[3], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var vector '%s = ??? ??? ???)\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("quat"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var quat '%s = %.4f %.4f %.4f %.4f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], pVal[3], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var quat '%s = ??? ??? ??? ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("float"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var float '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var float '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("string"):
		{
			const char** pVal = reinterpret_cast<const char**>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var string '%s = \"%s\") //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var string '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("symbol"):
		{
			StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var symbol '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var symbol '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			//fprintf(g_outHandle, "(var symbol '%s = %s)\n", StringIdToStringInternal(pDecl->m_declId), );
			break;
		}

		case SID("int32"):
		{
			int32_t* pVal = reinterpret_cast<int32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var int32 '%s = %d) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var int32 '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("uint64"):
		{
			uint64_t* pVal = reinterpret_cast<uint64_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var uint64 '%s = %llX) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var uint64 '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("timer"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var timer '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var timer '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("point"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var point '%s = %.2f %.2f %.2f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(var point '%s = ??? ??? ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("bound-frame"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(var bound-frame '%s = ??? Tell icemesh - i really doubt we should be here\n", StringIdToStringInternal(pDecl->m_declId));
			}
			else
			{
				fprintf(g_outHandle, "(var bound-frame '%s = (??? ??? ???) (??? ??? ??? ???) (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		default:
		{
			fprintf(g_outHandle, "(var '%s '%s = ???\n", StringIdToStringInternal(pDecl->m_declTypeId), StringIdToStringInternal(pDecl->m_declId));
			fprintf(g_outHandle, "Tell icemesh\n");
			break;
		}
	}
}

void PrintTag(SsDeclaration* pDecl)
{
	switch (pDecl->m_declTypeId)
	{
		case SID("boolean"):
		{
			uint32_t* pVal = reinterpret_cast<uint32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag boolean '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal == 1 ? "#t" : "#f", ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag boolean '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("string"):
		{
			char* pVal = reinterpret_cast<char*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "IMPLEMENT ME SID(string) Tell icemesh");
				//fprintf(g_outHandle, "(tag string '%s = \"%s\")")
			}
			else
			{
				fprintf(g_outHandle, "(tag string '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}

			break;
		}

		case SID("float"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag float '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag float '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("identifier"):
		{
			StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag identifier '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag identifier '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			
			break;
		}

		case SID("symbol"):
		{
			StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag symbol '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag symbol '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("int32"):
		{
			int32_t* pVal = reinterpret_cast<int32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag int32 '%s = %d) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag int32 '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("timer"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				fprintf(g_outHandle, "(tag timer '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				fprintf(g_outHandle, "(tag timer '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		default:
		{
			fprintf(g_outHandle, "(tag %s '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declTypeId), StringIdToStringInternal(pDecl->m_declId));
			fprintf(g_outHandle, "Tell icemesh\n");
			break;
		}
	}
}

void DumpState(SsState* pState)
{
	fprintf(g_outHandle, "  (state (\"%s\")\n", StringIdToStringInternal(pState->m_stateId));
	int64_t maxBlocks = pState->m_numSsOnBlocks;
	for (int32_t iBlocks = 0; iBlocks < maxBlocks; iBlocks++)
	{
		DumpBlocks(&pState->m_pSsOnBlocks[iBlocks]);
	}
	fprintf(g_outHandle, "  )\n");
}

void DumpBlocks(SsOnBlock* pBlock)
{
	const char* blockType = NULL;
	switch (pBlock->m_blockType)
	{
	case 0:
	{
		blockType = "start";
		break;
	}

		case 1:
		{
			//blockType = pBlock->m_name;
			blockType = "end";
			break;
		}

		case 2:
		{
			//blockType = "event";
			//fprintf(g_outHandle, "\t\tevent name: %s\n", StringIdToStringInternal(pBlock->m_blockId));
			char buf[0x20];
			snprintf(buf, 0x20, "event '%s", StringIdToStringInternal(pBlock->m_blockEventId));
			blockType = buf;
			break;
		}

		case 3:
		{
			blockType = "update";
			break;
		}

		case 4:
		{
			blockType = "virtual";
			break;
		}

		default:
		{
			blockType = "???";
			break;
		}
	}
	fprintf(g_outHandle, "    (on (%s)\n", blockType);
	for (int i = 0; i < pBlock->m_trackGroup.m_numTracks; i++)
	{
		PrintTrackGroup(&pBlock->m_trackGroup.m_aTracks[i]);
	}
	fprintf(g_outHandle, "    )\n");
}

void PrintTrackGroup(SsTrack* pTrack)
{
	fprintf(g_outHandle, "      (track (\"%s\")\n", StringIdToStringInternal(pTrack->m_trackId));
	int16_t tot = pTrack->m_totalLambdaCount;
	SsLambda* pTable = pTrack->m_pSsLambda;
	for (int16_t i = 0; i < tot; i++)
	{
		SsLambda* pdata = &pTable[i];
		ExecuteScriptCode(pdata->m_pScriptLambda);
	}
	fprintf(g_outHandle, "      )\n");
}

void DumpScript(StateScript* pStateScript)
{
	fprintf(g_outHandle, "  :location \"%s\":%d)\n", pStateScript->m_pDebugFileName, pStateScript->m_line);
	fprintf(g_outHandle, "  :initial-state '%s  //value @ %08llX\n", StringIdToStringInternal(pStateScript->m_initialStateId), ((uintptr_t) & (pStateScript->m_initialStateId) - g_moduleBase));
	//dump eventual ss-options
	SsOptions* pOptions = pStateScript->m_pSsOptions;
	if (pOptions != NULL && pOptions->m_pSymbolArray != NULL)
	{
		SymbolArray* pArr = pOptions->m_pSymbolArray;
		fprintf(g_outHandle, "  :options (new ss-options :wait-for-levels (make-symbol-array ");
		for (int32_t iSymbols = 0; iSymbols < pArr->m_numEntries; iSymbols++)
		{
			fprintf(g_outHandle, "'%s ", StringIdToStringInternal(pArr->m_pSymbols[iSymbols]));
		}
		fprintf(g_outHandle, "))\n");
	}
	//dump variables
	SsDeclarationList* pDeclTable = pStateScript->m_pSsDeclList;
	if (pDeclTable != NULL)
	{
		int32_t maxDecl = pDeclTable->m_numDeclarations;
		fprintf(g_outHandle, "  :declarations (declaration-list \n");
		for (int i = 0; i < maxDecl; i++)
		{
			SsDeclaration* pDecl = &pDeclTable->m_pDeclarations[i];
			if ((pDecl->m_isVar & 1) != 0)
			{
				fprintf(g_outHandle, "    ");
				PrintVariable(pDecl);
			}
			else
			{
				fprintf(g_outHandle, "    ");
				PrintTag(pDecl);
			}
		}
		fprintf(g_outHandle, "  )\n");
	}
	//dump states
	fprintf(g_outHandle, "\n");
	int16_t maxStates = pStateScript->m_stateCount;
	for (int16_t iStates = 0; iStates < maxStates; iStates++)
	{
		DumpState(&pStateScript->m_pSsStateTable[iStates]);
	}

}

void ExecuteScriptLambda(ScriptLambda* pLambda)
{
	if (pLambda)
	{
		ExecuteScriptCode(pLambda);
	}
}