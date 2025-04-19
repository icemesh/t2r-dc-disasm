/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#pragma once
#include <cinttypes>

#define SID(str) (ToStringId64(str))

typedef std::uint64_t StringId64;

inline constexpr StringId64 ToStringId64(const char* str);

constexpr StringId64 ToStringId64(const char* str)
{
	uint64_t base = 0xCBF29CE484222325;
	if (*str)
	{
		do {
			base = 0x100000001B3 * (base ^ *str++);
		} while (*str);
	}
	return base;
}
