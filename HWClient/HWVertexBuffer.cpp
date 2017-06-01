#include "pch.h"
#include "HWVertexBuffer.h"

HWVertexBuffer::HWVertexBuffer(HWBufferType type)
{
	useage = (GLenum)HWBufferUseage::Draw | (GLenum)type;
	length = 0;
	transfered_length = 0;
	data = nullptr;
	bind_type = HWBufferBindType::VertexBuffer;
	glGenBuffers(1, &pointer);
}

HWVertexBuffer::~HWVertexBuffer()
{
}

void HWVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, pointer);
	format.SetAttributes();
}

void HWVertexBuffer::SetFormat(HWVertexFormat form)
{
	format = form;
}
