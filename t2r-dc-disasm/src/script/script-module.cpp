/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "script-module.h"
#include "../stringid/sidbase.h"

#include "../dc/dc.h"
//used to dump state-scripts
#include "../dc/state-script/state-script.h"
#include "../ss-debug/state-script-debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <intrin.h> //bittest

extern uintptr_t g_moduleBase;
extern FILE* g_outHandle;

void Module::DumpEntry(Entry* pEntry)
{
	StringId64 scriptType = pEntry->m_typeId;
	switch (scriptType)
	{
		case SID("state-script"):
		{
			fprintf(g_outHandle, "(define-state-script %s\n", StringIdToStringInternal(pEntry->m_scriptId));
			DumpScript(reinterpret_cast<StateScript*>(pEntry->m_entryPtr));
			fprintf(g_outHandle, ")\n");
			break;
		}

		case SID("script-lambda"):
		{
			fprintf(g_outHandle, "(defun '%s\n", StringIdToStringInternal(pEntry->m_scriptId));
			ExecuteScriptLambda(reinterpret_cast<ScriptLambda*>(pEntry->m_entryPtr));
			fprintf(g_outHandle, ")\n");
			break;
		}

		case SID("int32"):
		{
			fprintf(g_outHandle, "    int32 '%s = %d\n", StringIdToStringInternal(pEntry->m_scriptId), *reinterpret_cast<int32_t*>(pEntry->m_entryPtr));
			break;
		}

		case SID("symbol"):
		{
			fprintf(g_outHandle, "    symbol '%s ", StringIdToStringInternal(pEntry->m_scriptId));
			fprintf(g_outHandle, "= '%s\n", StringIdToStringInternal(*reinterpret_cast<StringId64*>(pEntry->m_entryPtr)) );
			break;
		}

		case SID("float"):
		{
			fprintf(g_outHandle, "    float '%s = '%f\n", StringIdToStringInternal(pEntry->m_scriptId), *reinterpret_cast<float*>(pEntry->m_entryPtr));
			break;
		}

		case SID("boolean"):
		{
			fprintf(g_outHandle, "    boolean '%s = '%s\n", StringIdToStringInternal(pEntry->m_scriptId), *reinterpret_cast<bool*>(pEntry->m_entryPtr) == 1 ? "#t" : "#f");
			break;
		}

		default:
		{
			fprintf(g_outHandle, "Unhandled case @ 0x%016llX\n    '%s ", (reinterpret_cast<uintptr_t>(pEntry) - g_moduleBase), StringIdToStringInternal(scriptType));
			fprintf(g_outHandle, "'%s = ???\n", StringIdToStringInternal(pEntry->m_scriptId) );
			break;
		}
	}
	fprintf(g_outHandle, "\n");
}

Module::Module() : m_pLoadedFile(nullptr) {}

Module::Module(const char* filename)
{
	m_pLoadedFile = nullptr;
	FILE* fh;
	fopen_s(&fh, filename, "rb");
	if (fh)
	{
		fseek(fh, 0x0, SEEK_END);
		size_t fsize = ftell(fh);
		fseek(fh, 0x0, SEEK_SET);
		void* pFile = malloc(fsize);
		if (pFile)
		{
			fread(pFile, fsize, 0x1, fh);
			fclose(fh);
			m_pLoadedFile = pFile;
		}
		else
		{
			fprintf(g_outHandle, "Failed to allocate file\n");
			fclose(fh);
		}
	}
	else
	{
		fprintf(g_outHandle, "Could not open %s! - Does it even exist?\n", filename);
	}
}

Module::~Module()
{
	if (m_pLoadedFile != nullptr)
	{
		free(m_pLoadedFile);
	}
}

int32_t Module::Login()
{
	if (m_pLoadedFile)
	{
		std::uint8_t* pScript = reinterpret_cast<std::uint8_t*>(m_pLoadedFile);
		return ScriptLogin(pScript, pScript, 0x0, 0xFFFFFFFFFFFFFFFF);
	}
	else
	{
		fprintf(g_outHandle, "Error! Invalid file\n");
		return 0;
	}
}

//this can be improved... Am i going to ? probably not :^)
int32_t Module::ScriptLogin(uint8_t* pFile, uint8_t* pFile1, uint64_t lowerBound, uint64_t upperBound)
{
	//mov     rsi, [rdi]
	uint64_t magic = *(uint64_t*)pFile;
	//cmp     esi, 44433030
	if ((uint32_t)magic == 0x44433030)
	{
		if ((magic >> 0x20) == 1)
		{
			//movsxd  r10, dword ptr [rdi+8]
			int32_t v0 = *(int32_t*)(pFile + 0x8);
			//mov     bl, 1

			//mov     r11d, [rdi+r10]
			int32_t size = *(int32_t*)(pFile + v0);
			//mov     [rbp+size], r11

			//test    r11d, 1FFFFFFFh
			if (size & 0x1FFFFFFF)
			{
				//add     r10, rdi
				uint8_t* pUnk = pFile + v0;
				//lea     r8d, ds:0[r11*8] ; Load Effective Address
				int32_t max = size * 8;
				//xor     r14d, r14d      ; iPtr = 0
				int32_t iPtr = 0;
				//xor     r13d, r13d      ; Logical Exclusive OR
				int32_t byteIndex = 0;
				//xor     ebx, ebx        ; Logical Exclusive OR
				int32_t v1 = 0;
				//mov     [rbp+lowerBound], rdx
				//mov     [rbp+pFile_and_r10], r10
				//mov     [rbp+Max], r8
				int8_t r12d = 0x0;
				//eax
				int32_t v2 = 0x0;

				int32_t r15d = 0x0;

				uint64_t loc = 0;
				uint64_t tmp = 0;
				do {
					if (byteIndex < size)
					{
						//lea     r12d, [rbx+1]   ; Load Effective Address
						r12d = v1 + 1;
						//mov     esi, 0

						//mov     eax, r13d
						//movzx   eax, byte ptr [r10+rax+4] ; Move with Zero-Extend
						v2 = *(pUnk + byteIndex + 0x4);
						//cmp     r12d, 8         ; Compare Two Operands
						if (r12d == 8)
						{
							//setz    dl              ; Set Byte if Zero (ZF=1)
							r15d = 1;
							//cmovz   r12d, esi       ; Move if Zero (ZF=1)
							r12d = 0;
						}
						else
						{
							//movzx   r15d, dl        ; Move with Zero-Extend
							r15d = 0;
						}
						//bt      eax, ebx 
						if (_bittest(reinterpret_cast<long*>(&v2), v1))
						{
							//mov     rdx, [rdi+r14*8] ; loc = *(uint64_t*)(pFile+iPtr+0x8)
							loc = *(uint64_t*)(pFile + (iPtr * 0x8));//iPtr*sizeof(void*) platform dependant
							//mov     eax, 0
							tmp = 0;
							if (loc != 0)
							{
								if ((loc >= lowerBound && loc < upperBound))
								{
									//mov     rax, rdx
									tmp = loc;
								}
							}
							//add     rax, r9
							//mov     [rdi+r14*8], rax
							*(uint64_t*)(pFile + (iPtr * 0x8)) = (uint64_t)(pFile1 + tmp);
						}
						//add     r13d, r15d
						byteIndex += r15d;
						//inc     r14
						iPtr++;
						//mov     ebx, r12d
						v1 = r12d;
					}
					//cmp     r8d, r14d
				} while (max > iPtr);
				return 1;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			fprintf(g_outHandle, "Error! Wrong version number. (%d/%lld)\n", 1, (magic >> 0x20));
			return 0;
		}
	}
	else
	{
		fprintf(g_outHandle, "Error! Invalid magic number. (%08X/%016llX)\n", 0x44433030, (magic & 0xFFFFFFFF));
		return 0;
	}
}
