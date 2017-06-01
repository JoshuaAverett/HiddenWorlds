#pragma once

#include "Master.h"

class HWRandom
{
public:
	HWRandom();
	HWRandom(U64 seed);
	~HWRandom();

	U8 GetU8();
	U16 GetU16();
	U32 GetU32();
	U64 GetU64();

	S8 GetS8();
	S16 GetS16();
	S32 GetS32();
	S64 GetS64();

	F32 GetF32();
	F64 GetF64();

	void GetBytes(U8* out, S32 count);

private:
	U64 seed;
};

