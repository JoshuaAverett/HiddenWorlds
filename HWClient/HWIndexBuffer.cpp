#include "pch.h"
#include "HWIndexBuffer.h"

HWIndexBuffer::HWIndexBuffer()
{
	index_type = HWIndexType::UnsignedShort;
	useage = GL_STATIC_DRAW;
	length = 0;
	transfered_length = 0;
	data = nullptr;
	bind_type = HWBufferBindType::IndexBuffer;
	glGenBuffers(1, &pointer);
}

HWIndexBuffer::HWIndexBuffer(HWIndexType _type)
{
	index_type = _type;
	useage = GL_STATIC_DRAW;
	length = 0;
	transfered_length = 0;
	data = nullptr;
	bind_type = HWBufferBindType::IndexBuffer;
	glGenBuffers(1, &pointer);
}

HWIndexBuffer::~HWIndexBuffer()
{
}

HWIndexType HWIndexBuffer::GetIndexType() const
{
	return index_type;
}

S32 HWIndexBuffer::Count() const
{
	S32 elementLength = 2;
	switch(GetIndexType())
	{
	case HWIndexType::UnsignedByte:
		elementLength = 1;
		break;
	case HWIndexType::UnsignedShort:
		elementLength = 2;
		break;
	case HWIndexType::UnsignedInt:
		elementLength = 4;
		break;
	}

	return length / elementLength;
}
