/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#pragma once

#include <stdint.h>

struct ScriptLambda;

namespace DC
{
	enum OpCodes
	{
		kScriptCodeReturn = 0,
		kScriptCodeIAdd = 1,
		kScriptCodeISubtract = 2,
		kScriptCodeIMultiply = 3,
		kScriptCodeIDivide = 4,
		//floating point
		kScriptCodeFAdd = 5,
		kScriptCodeFSubtract = 6,
		kScriptCodeFMultiply = 7,
		kScriptCodeFDivide = 8,
		//load ops
		kLoadStaticInteger = 9,
		kLoadStaticFloat = 10,
		kLoadStaticPointer = 11,
		kLoadU16Immediate = 12,
		kLoadU32 = 13,
		kLoadFloat = 14,
		kLoadPointer = 15,
		//store ops
		kStoreInteger = 16,
		kStoreFloat = 17,
		kStorePointer = 18,
		//lookup values
		kLookupInteger = 19,
		kLookupFloat = 20,
		kLookupPointer = 21,
		//move ops
		kMoveInteger = 22,
		kMoveFloat = 23,
		kMovePointer = 24,
		//cast ops
		kCastInteger = 25,
		kCastFloat = 26,
		//call functions
		kCall = 27,
		kCallFf = 28,
		//integer cmp 
		kIeq = 29,
		kIgt = 30,
		kIgte = 31,
		kIlt = 32,
		kIlte = 33,
		//float ops
		kFeq = 34,
		kFgt = 35,
		kFgte = 36,
		kFlt = 37,
		kFlte = 38,
		//mod, abs 
		kImod = 39,
		kFmod = 40,
		kIabs = 41,
		kFabs = 42,
		//branches
		kGoto = 43,
		kLabel = 44,
		kBranch = 45,
		kBranchIf = 46,
		kBranchIfNot = 47,
		//logical ops
		kNot = 48,
		kBitwiseAnd = 49,
		kBitwiseNot = 50,
		kBitwiseIor = 51,
		kBitwiseXor = 52,
		kBitwiseNor = 53,
		kAnd = 54,
		kOr = 55,
		kInegate = 56,
		kFnegate = 57,
		kLoadParamCount = 58,
		//immediate ops ?
		kIaddImm = 59,
		kIsubtractImm = 60,
		kImultiplyImm = 61,
		kIdivideImm = 62,
		kLoadStaticI32Imm = 63,
		kLoadStaticFloatImm = 64,
		kLoadStaticPointerImm = 65,
		//
		kIash = 66,
		kMove = 67,
		//imm load ops
		kLoadStaticU32Imm = 68,
		kLoadStaticI8Imm = 69,
		kLoadStaticU8Imm = 70,
		kLoadStaticI16Imm = 71,
		kLoadStaticU16Imm = 72,
		kLoadStaticI64Imm = 73,
		kLoadStaticU64Imm = 74,
		//load ops
		kLoadI8 = 75,
		kLoadU8 = 76,
		kLoadI16 = 77,
		kLoadU16 = 78,
		kLoadI32 = 79,
		kLoadI64 = 80,
		kLoadU64 = 81,
		//store ops
		kStoreI8 = 82,
		kStoreU8 = 83,
		kStoreI16 = 84,
		kStoreU16 = 85,
		kStoreI32 = 86,
		kStoreU32 = 87,
		kStoreI64 = 88,
		kStoreU64 = 89,
		//
		kNotIeq = 90,
		kNotFeq = 91,
		kStoreArray = 92,
		kAssertPointer = 93
	};

	union Variant
	{
		int8_t			m_I8;
		uint8_t			m_U8;
		int16_t			m_I16;
		uint16_t		m_U16;
		uint16_t		m_F16;
		int32_t			m_I32;
		uint32_t		m_U32;
		float			m_F32;
		double			m_F64;
		int64_t			m_I64;
		uint64_t		m_U64;
	};
#define kMaxRegisters 128 //technically it should be 0x41
	struct StackFrame
	{
		Variant		m_registers[kMaxRegisters];
	};
}

DC::Variant ExecuteScriptCode(ScriptLambda* pLambda);

void DebugDumpRegisters(DC::StackFrame* pFrame);

void PrintCallArgs(DC::StackFrame* pFrame, int count);

