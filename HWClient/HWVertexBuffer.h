#pragma once

#include "HWBuffer.h"
#include "HWVertexFormat.h"

class HWVertexBuffer : public HWBuffer
{
public:
	HWVertexBuffer();
	HWVertexBuffer(HWBufferType type);
	~HWVertexBuffer();

	void Bind();

	void SetFormat(HWVertexFormat form);

private:
	HWVertexFormat format;
};
