#include "pch.h"
#include "HWBuffer.h"

HWBuffer::~HWBuffer()
{
	delete[] data;
	glDeleteBuffers(1, &pointer);
}

bool HWBuffer::Transfer()
{
	Bind();
	if(pointer)
	{
		// TODO: Make this smarter.
		if(length <= transfered_length)
		{
			glNamedBufferSubDataEXT(pointer, 0, length, data);
		}
		else
		{
			glNamedBufferDataEXT(pointer, length, data, useage);
		}
	}
	else
	{
		glNamedBufferDataEXT(pointer, length, data, useage);
		transfered_length = length;
	}
	return true;
}

void HWBuffer::BufferData(const void* _data, S32 _length)
{
	if(data)
	{
		U8* old_data = data;
		data = new U8[length + _length];
		memcpy(data, old_data, length);
		memcpy(data + length, _data, _length);
		delete[] old_data;
		length += _length;
	}
	else
	{
		length = _length;
		data = new U8[length];
		memcpy(data, _data, length);
	}
}

void HWBuffer::SetDataDirect(const void* _data, S32 _length)
{
	if(data)
	{
		delete[] data;
	}

	length = _length;
	data = new U8[length];
	memcpy(data, _data, length);
}

void HWBuffer::Bind() const
{
	glBindBuffer((GLenum)bind_type, pointer);
}
