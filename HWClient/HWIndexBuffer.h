#pragma once

#include "HWBuffer.h"

enum class HWIndexType
{
	UnsignedByte = GL_UNSIGNED_BYTE,
	UnsignedShort = GL_UNSIGNED_SHORT,
	UnsignedInt = GL_UNSIGNED_INT
};

class HWIndexBuffer : public HWBuffer
{
public:
	HWIndexBuffer();
	HWIndexBuffer(HWIndexType _type);
	~HWIndexBuffer();

	HWIndexType GetIndexType() const;
	S32 Count() const;

private:
	HWIndexType index_type;
};
