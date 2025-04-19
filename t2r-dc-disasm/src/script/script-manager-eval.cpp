/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "script-manager-eval.h"
#include "../dc/state-script/state-script.h"
#include "script-module.h"
#include "../stringId/sidbase.h"

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <intrin.h> //bittest
#include <math.h> //fmodf


typedef uint64_t ScriptInstruction;

extern DC::StackFrame* g_stackFrame;
extern DC::StackFrame* g_unkArea;

extern uintptr_t g_moduleBase;
extern FILE* g_outHandle;
int g_globalIdx = 0;

DC::Variant ExecuteScriptCode(ScriptLambda* pLambda)
{
	memset(g_stackFrame, 0x0, sizeof(DC::StackFrame) * 2);
	int64_t idx = 0;
	int64_t tmpIdx = 0;
	int numberOfMovBeforeCall = 0;
	uint8_t* pOpcode = pLambda->m_pOpcode;
	DC::Variant* pSymbolTbl = reinterpret_cast<DC::Variant*>(pLambda->m_pSymbols);
	fprintf(g_outHandle, "        (\n");
	int iLoc = 0;
	//bool opcodeFlagSet = pLambda->m_instructionFlag != 0xDEADBEEF1337F00D;
	while (1)
	{
		uint8_t opcode = pOpcode[sizeof(ScriptInstruction) * idx];
		uint8_t dstReg = pOpcode[sizeof(ScriptInstruction) * idx + 0x1];
		uint8_t operand1 = pOpcode[sizeof(ScriptInstruction) * idx + 0x2];
		uint8_t operand2 = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
		tmpIdx = idx + 0x1;
		fprintf(g_outHandle, "            loc_%d - CODE: 0x%.08llX %02X %02X %02X %02X ", iLoc++, (reinterpret_cast<uintptr_t>(&pOpcode[sizeof(ScriptInstruction) * idx]) - g_moduleBase), opcode, dstReg, operand1, operand2);
		switch (opcode)
		{
			case DC::kScriptCodeReturn:
			{
				DC::Variant ret = g_stackFrame->m_registers[operand1];
				fprintf(g_outHandle, "DC::kScriptCodeReturn: 0x%.16llX\n", ret.m_U64);
				fprintf(g_outHandle, "        )\n");
				return ret;
			}
			case DC::kScriptCodeIAdd:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 + g_stackFrame->m_registers[operand2].m_I64;
				idx = tmpIdx;
				fprintf(g_outHandle, "DC::kScriptCodeIAdd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				break;
			}

			case DC::kScriptCodeISubtract:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 - g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kScriptCodeISubtract: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeIMultiply:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 * g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kScriptCodeIMultiply: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeIDivide:
			{
				//g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 / g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kScriptCodeIDivide: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFAdd:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 + g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kScriptCodeFAdd: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFSubtract:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 - g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kScriptCodeFSubtract: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFMultiply:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 * g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kScriptCodeFMultiply: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFDivide:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 / g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kScriptCodeFDivide: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticInteger:
				//			case DC::kLoadStaticPointer:
			{
				DC::Variant val = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64];
				//g_stackFrame->m_registers[dstReg] = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64];
				g_stackFrame->m_registers[dstReg] = val;
				fprintf(g_outHandle, "DC::kLoadStaticInteger: R%d, [pSymbolTbl, R%d] -> 0x%016llX\n", dstReg, operand1, g_stackFrame->m_registers[dstReg].m_I64);
				//fprintf(g_outHandle, "Registers:  R%d -> 0x%.16llX\n\t\t\t\t\t\t\tR%d -> 0x%.16llX\n", dstReg, g_stackFrame->m_registers[dstReg].m_I64, operand1, g_stackFrame->m_registers[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticFloat:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64].m_U32;
				fprintf(g_outHandle, "DC::kLoadStaticFloat: S%d, [pSymbolTbl, S%d]\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticPointer:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64].m_U64;
				fprintf(g_outHandle, "DC::kLoadStaticPointer: R%d, [pSymbolTbl, R%d] // offset: %llX\n", dstReg, operand1, g_stackFrame->m_registers[dstReg].m_U64 - g_moduleBase);
				//fprintf(g_outHandle, "Registers:  R%d -> 0x%.16llX\n\t\t\t\t\t\t\tR%d -> 0x%.16llX\n", dstReg, g_stackFrame->m_registers[dstReg].m_I64, operand1, g_stackFrame->m_registers[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU16Immediate:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = (operand1 | (operand2 << 0x8));
				fprintf(g_outHandle, "DC::kLoadU16Immediate: R%d, %d | (%d << 0x8)\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU32:
			{
				//	g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32;
				fprintf(g_outHandle, "DC::kLoadU32: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadFloat:
			{
				//	g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_I64;
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32;
				fprintf(g_outHandle, "DC::kLoadFloat: S%d, [R%d]\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadPointer:
			case DC::kLoadI64:
			case DC::kLoadU64:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				fprintf(g_outHandle, "DC::kLoadPointer/kLoadI64/kLoadU64: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreInteger:
			case DC::kStoreU64:
			{
				uint64_t data = g_stackFrame->m_registers[operand2].m_U64;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_U64 = data;
				fprintf(g_outHandle, "DC::kStoreInteger/kStoreU64: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreFloat:
			{
				uint32_t data = g_stackFrame->m_registers[operand2].m_U32;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_I32 = data;
				fprintf(g_outHandle, "DC::kStoreFloat: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStorePointer:
			{
				uint64_t data = g_stackFrame->m_registers[operand2].m_U64;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_U64 = data;
				fprintf(g_outHandle, "DC::kStorePointer: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kLookupInteger:
			case DC::kLookupFloat:
			case DC::kLookupPointer:
			{
				//fprintf(g_outHandle, "DC::kLookupInteger/kLookupFloat: R%d R%d -> 0x%016llX\n", opcode, dstReg, operand1, operand2, dstReg, operand1, pSymbolTbl[operand1].m_U32);
				fprintf(g_outHandle, "DC::kLookupInteger/kLookupFloat: R%d R%d -> hashid: 0x%016llX(%s)\n", dstReg, operand1, pSymbolTbl[operand1].m_U64, StringIdToStringInternal(pSymbolTbl[operand1].m_U64));
				//	g_stackFrame->m_registers[dstReg].m_U64 = 0;
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I64;
				idx = tmpIdx;
				break;
			}
			/*
			case DC::kLookupPointer:
			{
				fprintf(g_outHandle, "DC::kLookupPointer: R%d R%d -> 0x%08X -> %s\n", opcode, dstReg, operand1, operand2, dstReg, operand1, pSymbolTbl[operand1].m_U32, StringIdToStringInternal(pSymbolTbl[operand1].m_U32) );
				//	g_stackFrame->m_registers[dstReg].m_U64 = 0;
					g_stackFrame->m_registers[dstReg].m_U32 = pSymbolTbl[operand1].m_U32;
					idx = tmpIdx;
					break;
			}
			*/
			case DC::kMoveInteger:
			case DC::kMovePointer:
			case DC::kMove:
			{
				//g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				g_stackFrame->m_registers[dstReg] = g_stackFrame->m_registers[operand1];
				//mov     rax, [rbp+r15*8+s+208h]
				//mov     [rbp+rsi*8+s+208h], rax
				//g_unkArea->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				//g_unkArea->m_registers[dstReg] = g_stackFrame->m_registers[operand1];
				fprintf(g_outHandle, "DC::kMoveInteger/kMovePointer/kMove: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kMoveFloat:
			{
				g_stackFrame->m_registers[dstReg].m_I32 = g_stackFrame->m_registers[operand1].m_I32;
				fprintf(g_outHandle, "DC::kMoveInteger/kMovePointer/kMove: S%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCastInteger:
			{
				g_stackFrame->m_registers[dstReg].m_I32 = (int32_t)g_stackFrame->m_registers[dstReg].m_F32;
				fprintf(g_outHandle, "DC::kCastInteger: R%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCastFloat:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = (float)g_stackFrame->m_registers[dstReg].m_F64;
				fprintf(g_outHandle, "DC::kCastFloat: S%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCall:
			{
				ScriptLambda* pValue = reinterpret_cast<ScriptLambda*>(g_stackFrame->m_registers[operand1].m_U64);
				if (pValue)
				{
					//g_stackFrame->m_registers[dstReg] = ExecuteScriptCode(pValue);
					fprintf(g_outHandle, "DC::kCall: ( %s ", StringIdToStringInternal(g_stackFrame->m_registers[operand1].m_U64));
					int8_t argc = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
					PrintCallArgs(g_stackFrame, argc);
					fprintf(g_outHandle, ")\n");
				}
				else
				{
					fprintf(g_outHandle, "DC::kCall: Unable to call defun");
				}
				//		DebugDumpRegisters(g_stackFrame);
				//		DebugDumpRegisters(g_unkArea);
				idx = tmpIdx;
				numberOfMovBeforeCall = 0;
				break;
			}

			case DC::kCallFf:
			{
				uint64_t data = g_stackFrame->m_registers[operand1].m_U64;
				fprintf(g_outHandle, "DC::kCallFf: ( %s ", StringIdToStringInternal(g_stackFrame->m_registers[operand1].m_U64));

				int8_t argc = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
				PrintCallArgs(g_stackFrame, argc);
				fprintf(g_outHandle, ")\n");
				//	DebugDumpRegisters(g_stackFrame);
				//	DebugDumpRegisters(g_unkArea);
				numberOfMovBeforeCall = 0;
				idx = tmpIdx;
				break;
			}

			case DC::kIeq:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 == g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kIeq: R%d == R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIgt:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 > g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kIgt: R%d > R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIgte:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 >= g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kIgte: R%d >= R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIlt:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 < g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kIlt: R%d < R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIlte:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 <= g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kIlte: R%d <= R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kFeq:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 == g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kFeq: S%d == S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFgt:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 > g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kFgt: S%d > S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFgte:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 >= g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kFgte: S%d >= S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFlt:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 < g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kFlt: S%d < S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFlte:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 <= g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kFlte: S%d <= S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kImod:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 % g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kImod: R%d mod R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kFmod:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = fmodf(g_stackFrame->m_registers[operand1].m_F32, g_stackFrame->m_registers[operand2].m_F32);
				fprintf(g_outHandle, "DC::kFmod: fmodf(S%d S%d)\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIabs:
			{
				int64_t res = -g_stackFrame->m_registers[operand1].m_I64;
				if (res < 1)
				{
					res = g_stackFrame->m_registers[operand1].m_I64;
				}
				fprintf(g_outHandle, "DC::kIabs: abs(R%d)\n", operand1);
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFabs:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = fabsf(g_stackFrame->m_registers[operand1].m_F32);
				fprintf(g_outHandle, "DC::kFabs: S%d fabsf(S%d)\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBranch:
			{
				//idx = dstReg | (uint32_t)(operand2 << 0x8);
				//fprintf(g_outHandle, "DC::kBranch: B loc_%lld\n", dstReg | (uint32_t)(operand2 << 0x8));
				fprintf(g_outHandle, "DC::kBranch: B loc_%d\n", dstReg | (uint32_t)(operand2 << 0x8));
				idx = tmpIdx;
				break;
			}

			case DC::kBranchIf:
			{
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					//tmpIdx = dstReg | (uint32_t)operand2 << 0x8;
					//fprintf(g_outHandle, "Taken DC::kBranchIf: B loc_%lld\n", dstReg | (uint32_t)operand2 << 0x8);
					fprintf(g_outHandle, "DC::kBranchIf: B loc_%d\n", dstReg | (uint32_t)operand2 << 0x8);
				}
				else
				{
					//fprintf(g_outHandle, "Not Taken DC::kBranchIf: B loc_%lld\n", tmpIdx);
					fprintf(g_outHandle, "DC::kBranchIf: B loc_%lld\n", tmpIdx);
				}
				idx = tmpIdx;
				break;
			}

			case DC::kBranchIfNot:
			{
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					//tmpIdx = dstReg | (uint32_t)operand2 << 0x8;
					//fprintf(g_outHandle, "Taken DC::kBranchIfNot: B loc_%lld\n", tmpIdx);
					fprintf(g_outHandle, "DC::kBranchIfNot: B loc_%d\n", dstReg | (uint32_t)operand2 << 0x8);
				}
				else
				{
					fprintf(g_outHandle, "DC::kBranchIfNot: B loc_%lld\n", tmpIdx);
				}
				idx = tmpIdx;
				break;
			}

			case DC::kNot:
			{
				//	I8 res = -(g_stackFrame->m_registers[operand1].m_I64 == 0);
				g_stackFrame->m_registers[dstReg].m_I64 = -(g_stackFrame->m_registers[operand1].m_I64 == 0);;
				fprintf(g_outHandle, "DC::kNot: R%d R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseAnd:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 & g_stackFrame->m_registers[operand2].m_U64;
				fprintf(g_outHandle, "DC::kBitwiseAnd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseNot:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = ~g_stackFrame->m_registers[operand1].m_U64;
				fprintf(g_outHandle, "DC::kBitwiseAnd: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseIor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 | g_stackFrame->m_registers[operand2].m_U64;
				fprintf(g_outHandle, "DC::kBitwiseIor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseXor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 ^ g_stackFrame->m_registers[operand2].m_U64;
				fprintf(g_outHandle, "DC::kBitwiseXor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseNor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = ~(g_stackFrame->m_registers[operand1].m_U64 | g_stackFrame->m_registers[operand2].m_U64);
				fprintf(g_outHandle, "DC::kBitwiseNor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kAnd: //this is probably the implementation of '&&'
			{
				uint8_t res = 0;
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					if (g_stackFrame->m_registers[operand2].m_U64)
					{
						res = 1;
					}
				}
				fprintf(g_outHandle, "DC::kAnd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kOr:  //implementation of '||'
			{
				uint8_t res = 1;
				if (!g_stackFrame->m_registers[operand1].m_U64)
				{
					res = g_stackFrame->m_registers[operand2].m_U64 != 0;
				}
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				fprintf(g_outHandle, "DC::kOr: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kInegate:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = -g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kInegate: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kFnegate:
			{
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32 ^ 0x80000000;
				fprintf(g_outHandle, "DC::kFnegate: R%d, R%d ^ 0x80000000\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadParamCount:
			{
				//TODO: this is partially correct
				//the parameters gets passed in input to the function
				g_stackFrame->m_registers[dstReg].m_U32 = 65LL;
				fprintf(g_outHandle, "DC::kLoadParamCount: R%d, #%X\n", dstReg, 65);
				idx = tmpIdx;
				break;
			}

			case DC::kIaddImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = operand2 + g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kIaddImm: R%d, R%d, %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIsubtractImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 - operand2;
				fprintf(g_outHandle, "DC::kIsubtractImm: R%d, R%d - %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kImultiplyImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = operand2 * g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kImultiplyImm: R%d, R%d * %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIdivideImm:
			{
				//g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 / operand2;
				fprintf(g_outHandle, "DC::kIdivideImm: R%d, R%d / %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI32Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I32;
				fprintf(g_outHandle, "DC::kLoadStaticI32Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticFloatImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U32;
				fprintf(g_outHandle, "DC::kLoadStaticFloatImm: S%d, %f\n", dstReg, pSymbolTbl[operand1].m_F32);
				idx = tmpIdx;
				break;
			}
			
			case DC::kLoadStaticPointerImm:
			{
				//g_stackFrame->m_registers[dstReg] = pSymbolTbl[operand1];
				g_stackFrame->m_registers[dstReg].m_U64 = (pSymbolTbl[operand1].m_U64 - g_moduleBase);
				fprintf(g_outHandle, "DC::kLoadStaticPointerImm: R%d, %.16llX // offset %llX\n", dstReg, pSymbolTbl[operand1].m_U64, (pSymbolTbl[operand1].m_U64 - g_moduleBase));
				idx = tmpIdx;
				break;
			}
			
			//case DC::kLoadStaticPointerImm:
			case DC::kLoadStaticI64Imm:
			case DC::kLoadStaticU64Imm:
			{
				g_stackFrame->m_registers[dstReg] = pSymbolTbl[operand1];
				//fprintf(g_outHandle, "DC::kLoadStaticPointerImm/kLoadStaticI64Imm/kLoadStaticU64Imm: R%d, %.16llX(%s) () // offset %llX\n", dstReg, pSymbolTbl[operand1].m_U64, StringIdToStringInternal(pSymbolTbl[operand1].m_U64), g_stackFrame->m_registers[dstReg].m_U64 - g_moduleBase);
				fprintf(g_outHandle, "DC::kLoadStaticI64Imm/kLoadStaticU64Imm: R%d, %.16llX(%s)\n", dstReg, pSymbolTbl[operand1].m_U64, StringIdToStringInternal(pSymbolTbl[operand1].m_U64));
				idx = tmpIdx;
				break;
			}

			case DC::kIash:
			{
				int64_t hash = g_stackFrame->m_registers[operand1].m_I64 >> -(char)g_stackFrame->m_registers[operand2].m_I8;
				if (g_stackFrame->m_registers[operand2].m_I64 >= 0)
				{
					hash = g_stackFrame->m_registers[operand1].m_I64 << g_stackFrame->m_registers[operand2].m_I8;
				}
				g_stackFrame->m_registers[dstReg].m_I64 = hash;
				fprintf(g_outHandle, "DC::kIash: R%d, %.16llX\n", dstReg, hash);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU32Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U32;
				fprintf(g_outHandle, "DC::kLoadStaticU32Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI8Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I8;
				fprintf(g_outHandle, "DC::kLoadStaticI8Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I8);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU8Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U8;
				fprintf(g_outHandle, "DC::kLoadStaticU8Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U8);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI16Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I16;
				fprintf(g_outHandle, "DC::kLoadStaticI16Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I16);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU16Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U16;
				fprintf(g_outHandle, "DC::kLoadStaticU16Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U16);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI8:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kLoadI8: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU8:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kLoadU8: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI16:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kLoadI16: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU16:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kLoadU16: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI32:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				fprintf(g_outHandle, "DC::kLoadI32: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI8:
			{
				int8_t val = g_stackFrame->m_registers[operand2].m_I8;
				g_stackFrame->m_registers[operand1].m_I8 = val;
				g_stackFrame->m_registers[dstReg].m_I8 = val;
				fprintf(g_outHandle, "DC::kStoreI8: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU8:
			{
				uint8_t val = g_stackFrame->m_registers[operand2].m_U8;
				g_stackFrame->m_registers[operand1].m_U8 = val;
				g_stackFrame->m_registers[dstReg].m_U8 = val;
				fprintf(g_outHandle, "DC::kStoreU8: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI16:
			{
				int16_t val = g_stackFrame->m_registers[operand2].m_I16;
				g_stackFrame->m_registers[operand1].m_I16 = val;
				g_stackFrame->m_registers[dstReg].m_I16 = val;
				fprintf(g_outHandle, "DC::kStoreI16: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU16:
			{
				uint16_t val = g_stackFrame->m_registers[operand2].m_U16;
				g_stackFrame->m_registers[operand1].m_U16 = val;
				g_stackFrame->m_registers[dstReg].m_U16 = val;
				fprintf(g_outHandle, "DC::kStoreU16: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI32:
			{
				int32_t val = g_stackFrame->m_registers[operand2].m_I32;
				g_stackFrame->m_registers[operand1].m_I32 = val;
				g_stackFrame->m_registers[dstReg].m_I32 = val;
				fprintf(g_outHandle, "DC::kStoreI32: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU32:
			{
				uint32_t val = g_stackFrame->m_registers[operand2].m_U32;
				g_stackFrame->m_registers[operand1].m_U32 = val;
				g_stackFrame->m_registers[dstReg].m_U32 = val;
				fprintf(g_outHandle, "DC::kStoreU32: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI64:
			{
				int64_t val = g_stackFrame->m_registers[operand2].m_I64;
				g_stackFrame->m_registers[operand1].m_I64 = val;
				g_stackFrame->m_registers[dstReg].m_I64 = val;
				fprintf(g_outHandle, "DC::kStoreI64: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kNotIeq:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 != g_stackFrame->m_registers[operand2].m_I64;
				fprintf(g_outHandle, "DC::kStoreI64: R%d, R%d != R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kNotFeq:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 != g_stackFrame->m_registers[operand2].m_F32;
				fprintf(g_outHandle, "DC::kStoreI64: S%d, S%d != S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreArray:
			{
				//TODO: implement me
				fprintf(g_outHandle, "DC::kStoreArray: Tell icemesh\n");
				//	__debugbreak();
				idx = tmpIdx;
				break;
			}

			case DC::kAssertPointer:
			{
				fprintf(g_outHandle, "DC::kAssertPointer:\n");
				//	__debugbreak();
				idx = tmpIdx;
				break;
			}

			default:
			{
				fprintf(g_outHandle, "RunScript: Unknown opcode '0x%X' encountered!\n", opcode);
				DC::Variant ret;
				ret.m_I8 = 0;
				return ret;
			}
		} //end switch
	} // end while
}

void DebugDumpRegisters(DC::StackFrame* pFrame)
{
	if (pFrame)
	{
		fprintf(g_outHandle, "R%d : [0x%.16llX]\n", 49, pFrame->m_registers[49].m_U64);
		fprintf(g_outHandle, "R%d : [0x%.16llX]\n", 50, pFrame->m_registers[50].m_U64);
		fprintf(g_outHandle, "R%d : [0x%.16llX]\n", 51, pFrame->m_registers[51].m_U64);
		fprintf(g_outHandle, "R%d : [0x%.16llX]\n", 52, pFrame->m_registers[52].m_U64);
		/*
		for (int i = 0; i < kMaxRegisters; i++)
		{
			if (pFrame->m_registers[i].m_U64 != 0)
			{
				fprintf(g_outHandle, "R%d : [0x%.16llX]\n", i, pFrame->m_registers[i].m_U64);
			}
		}
		*/
	}
}

void PrintCallArgs(DC::StackFrame* pFrame, int count)
{
	if (count > 0)
	{
		/*
		int regIdx = 49;
		fprintf(g_outHandle, "'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
		regIdx = 51;
		for (int i = 1; i < count; i++)
		{
			fprintf(g_outHandle, "'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
			regIdx++;
		}
		*/
	//	fprintf(g_outHandle, "'%s(", StringIdToStringInternal(pFrame->m_registers[0].m_U64));
		
		int regIdx = 49;
		for (int i = 0; i < count; i++)
		{
			fprintf(g_outHandle, "'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
			regIdx++;
		}
	}
}