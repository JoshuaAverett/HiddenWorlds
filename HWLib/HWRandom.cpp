#include "HWRandom.h"

#include <intrin.h>
#include <string>

HWRandom::HWRandom()
{
	seed = __rdtsc();
}

HWRandom::HWRandom(U64 _seed)
{
	seed = _seed;
}

HWRandom::~HWRandom()
{
}

U8 HWRandom::GetU8()
{
	U16 out;
	while(!_rdrand16_step(&out));
	return (U8)(out & 0xFF);
}

U16 HWRandom::GetU16()
{
	U16 out;
	while(!_rdrand16_step(&out));
	return out;
}

U32 HWRandom::GetU32()
{
	unsigned int out;
	while(!_rdrand32_step(&out));
	return out;
}

U64 HWRandom::GetU64()
{
	U64 out;
	while(!_rdrand64_step(&out));
	return out;
}

S8 HWRandom::GetS8()
{
	return static_cast<S8>(GetU8());
}

S16 HWRandom::GetS16()
{
	return static_cast<S16>(GetU16());
}

S32 HWRandom::GetS32()	
{
	return static_cast<S32>(GetU32());
}

S64 HWRandom::GetS64()
{
	return static_cast<S16>(GetU64());
}

F32 HWRandom::GetF32()
{
	return 0.0f;
}

F64 HWRandom::GetF64()
{
	return 0.0;
}

void HWRandom::GetBytes(U8* out, S32 count)
{
	U64 offset = 0;
	while(count >= 8)
	{
		U64 data = GetU64();
		memcpy(out + offset, &data, sizeof(U64));
		count -= 8;
		offset += 8;
	}
	if(count >= 4)
	{
		U32 data = GetU32();
		memcpy(out + offset, &data, sizeof(U32));
		count -= 4;
		offset += 4;
	}
	if(count >= 2)
	{
		U16 data = GetU16();
		memcpy(out + offset, &data, sizeof(U16));
		count -= 2;
		offset += 2;
	}
	if(count >= 1)
	{
		U16 data = GetU16();
		memcpy(out + offset, &data, 1);
	}
}