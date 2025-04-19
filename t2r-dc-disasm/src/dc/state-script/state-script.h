/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#pragma once
#include "../../stringid/stringid.h"

struct SsDeclarationList;
struct SsDeclaration;
struct StateScript;
struct SsOptions;
struct SymbolArray;
struct SsState;
struct SsTrackGroup;
struct SsOnBlock;
struct SsTrack;
struct SsLambda;
struct ScriptLambda;

struct StateScript //0x50
{
	StringId64				m_stateScriptId;		///< <c>0x00</c>: StringId64 of the script name
	SsDeclarationList*		m_pSsDeclList;			///< <c>0x08</c>: ptr to the declaration list
	StringId64				m_initialStateId;		///< <c>0x10</c>: StringId64 of the initial state
	SsOptions*				m_pSsOptions;			///< <c>0x18</c>: ptr to the SsOptions
	uint64_t				field_20;				///< <c>0x20</c>: always 0 ?
	SsState*				m_pSsStateTable;		///< <c>0x28</c>: ptr to the SsState Table
	int16_t					m_numStates;			///< <c>0x30</c>: number of states
	//start of debug info
	int16_t					m_line;					///< <c>0x32</c>: this is a line number thats get displayed in the debug display
	uint32_t				field_34;				///< <c>0x34</c>: padding probably
	const char*				m_pDebugFileName;		///< <c>0x38</c>: the debug filename eg: t2r/src/game/scriptx/ss/ss-tag-as-hero.dcx
	const char*				m_pErrorName;			///< <c>0x40</c>: used to store any error if any
	uint64_t				m_padding;				///< <c>0x48</c>: padding probably
};

struct SsDeclarationList //0x10
{
	int32_t					m_unkNumber;			///< <c>0x00</c>: unk number
	int32_t					m_numDeclarations;		///< <c>0x04</c>: number of declared vars in the table
	SsDeclaration*			m_pDeclarations;		///< <c>0x08</c>: ptr to the list of declarations
};

struct SsDeclaration //0x30
{
	StringId64				m_declId;				///< <c>0x00</c>: StringId of the declaration name
	uint64_t				m_padding;				///< <c>0x08</c>: padding probably
	StringId64				m_declTypeId;			///< <c>0x10</c>: StringId of the declaration type eg: boolean, int32, float etc..
	int16_t					m_unk;					///< <c>0x18</c>: unk number
	int16_t					m_isVar;				///< <c>0x1A</c>: is variable ?
	uint32_t				unk3;					///< <c>0x1C</c>: always 0 ?
	void*					m_pDeclValue;			///< <c>0x20</c>: ptr to the declaration value
	uint64_t				m_unkNumber;			///< <c>0x28</c>: always 0x80 ?
};

struct SsOptions //0x50
{
	uint8_t					m_imLazy[0x18];			///< <c>0x00</c>: self explanatory
	SymbolArray*			m_pSymbolArray;			///< <c>0x18</c>: ptr to the symbol array
	uint8_t					m_imLazyPt2[0x18];		///< <c>0x20</c>: self explanatory always 0 ?
	int64_t					m_unkNumber;			///< <c>0x38</c>: unk number
	uint8_t					m_imLazyPt3[0x10];		///< <c>0x20</c>: self explanatory always 0 ?
};

struct SymbolArray //0x10
{
	int32_t					m_numEntries;			///< <c>0x00</c>: number of entries 
	uint32_t				m_unk;					///< <c>0x04</c>: always 0 ?
	StringId64*				m_pSymbols;				///< <c>0x08</c>: ptr to the symbols
};

struct SsState //0x18
{
	StringId64				m_stateId;				///< <c>0x00</c>: StringId64 of the state name
	int64_t					m_numSsOnBlocks;		///< <c>0x08</c>: numSsOnBlocks
	SsOnBlock*				m_pSsOnBlocks;			///< <c>0x10</c>: ptr to the SsOnBlocks table
};

struct SsOnBlock //0x50
{
	int32_t					m_blockType;			///< <c>0x00</c>: //on start || on update || on event etc
	uint32_t				m_unkNumber;			///< <c>0x04</c>: unk number
	StringId64				m_blockEventId;			///< <c>0x08</c>: UNSURE. Can be null. if its null there's no script lambda ptr
	void*					m_pScriptLambda;		///< <c>0x10</c>: ptr to the script Lambda
	uint64_t				unk;
	uint16_t				m_unkNumber2;			///< <c>0x20</c>: unk number
	int16_t					m_numTracks;			///< <c>0x22</c>: num tracks
	uint32_t				m_padding;				///< <c>0x1C</c>: padding probably
	SsTrack*				m_pTrack;				///< <c>0x20</c>: ptr to a track
	const char*				m_name;					///< <c>0x30</c>: eg: ss-fp-test initial (on (start))
	uint64_t				field_38;               ///< <c>0x38</c>:
	uint64_t				field_40;               ///< <c>0x40</c>:
	uint64_t				field_48;               ///< <c>0x48</c>:
};

struct SsTrack //0x18
{
	StringId64				m_trackId;				///< <c>0x00</c>: StringId64 of the track name
	uint16_t				field_8;				///< <c>0x08</c>: always 0?
	int16_t					m_totalLambdaCount;		///< <c>0x0A</c>: unsure
	uint32_t				m_padding;				///< <c>0x0C</c>: padding probably
	SsLambda*				m_pSsLambda;			///< <c>0x10</c>: ptr to the SsLambda table
};

struct SsLambda //0x10
{
	ScriptLambda*			m_pScriptLambda;		///< <c>0x00</c>: ptr to the script lambda of the track
	uint64_t				m_unkNumber;			///< <c>0x08</c>: UNSURE: size maybe ?
};

struct ScriptLambda //0x40 || 0x48 unsure
{
	uint8_t*				m_pOpcode;				///< <c>0x00</c>: ptr to the first opcode of the script
	uint64_t*				m_pSymbols;				///< <c>0x08</c>: ptr to the symbol table
	StringId64				m_typeId;				///< <c>0x10</c>: usually SID("function")
	uint64_t				m_unkNumber;			///< <c>0x18</c>: no idea what this is used for
	uint64_t				m_unk;					///< <c>0x20</c>: always 0?
	uint64_t				m_instructionFlag;		///< <c>0x28</c>: 0xDEADBEEF1337F00D
	uint8_t					m_unk2;					///< <c>0x2C</c>: always 0 ?
	uint32_t				m_unkNumber3;			///< <c>0x34</c>: alwasy 0x10 ?
	uint64_t				m_unkSid;				///< <c>0x38</c>: Unknown StringId64
};