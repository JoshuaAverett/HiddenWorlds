#pragma once

#include "pch.h"
#include "HWGL.h"

enum class HWBufferUseage : GLenum
{
	 Draw = 0,
	 Read = 1,
	 Copy = 2
};

enum class HWBufferType : GLenum
{
	Stream = GL_STREAM_DRAW,
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW
};

enum class HWBufferBindType : GLenum
{
	 VertexBuffer = GL_ARRAY_BUFFER,
	 IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
};

class HWBuffer
{
public:
	~HWBuffer();

	// Sends the data to GL
	bool Transfer();

	// Loads data into the buffer
	void BufferData(const void* data, S32 _length);

	void SetDataDirect(const void* data, S32 length);

	// Binds this buffer
	void Bind() const;

protected:
	GLuint pointer;
	U8* data;
	GLsizei length;
	GLenum useage;
	GLsizei transfered_length;
	HWBufferBindType bind_type;
};
