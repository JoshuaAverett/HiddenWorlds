#include "pch.h"
#include "HWTexture.h"
#include "../HWLib/HWFile.h"

HWTexture::HWTexture()
{
	glGenTextures(1, &texture);
}

HWTexture::HWTexture(S32 width, S32 height, S32 depth)
{
	glGenTextures(1, &texture);
	S32 size = width * height * depth / 8;
	bitmap = new U8[size];
	memset(bitmap, 255, size);
	this->width = width;
	this->height = height;
	this->depth = depth;
	format = depth == 24 ? GL_BGR : GL_LUMINANCE8;
	is_dirty = true;
}

HWTexture::~HWTexture()
{
	glDeleteTextures(1, &texture);
	delete[] bitmap;
}

bool HWTexture::Read(const std::string& path)
{
	U8 header[54];

	HWFile file = HWFile(path, HWFileOperation::Read);
	file.Read(header, 54);

	if(header[0] != 'B' || header[1] != 'M')
	{
		return false;
	}

	U32 data_pos = header[0x0A] | (header[0x0B] << 8) | (header[0x0C] << 16) | (header[0x0D] << 24);
	U32 size = header[0x22] | (header[0x23] << 8) | (header[0x24] << 16) | (header[0x25] << 24);
	width = header[0x12] | (header[0x13] << 8) | (header[0x14] << 16) | (header[0x15] << 24);
	height = header[0x16] | (header[0x17] << 8) | (header[0x18] << 16) | (header[0x19] << 24);

	if(size == 0)
	{
		size = 3 * width * height;
	}

	if(data_pos == 0)
	{
		data_pos = 54;
	}

	bitmap = new U8[size];
	file.Read(bitmap, size);
	format = GL_BGR;
	depth = 3;

	is_dirty = true;

	return true;
}

void HWTexture::FromPixels(const U8* data, S32 size, S32 width, S32 height, GLenum format)
{
	bitmap = new U8[size];
	std::copy(&data[0], &data[size - 1], bitmap);
	this->format = format;
	this->width = width;
	this->height = height;
	this->depth = 3; // TODO: this needs to be generated from the format
	is_dirty = true;
}

void HWTexture::Transfer()
{
	if(is_dirty)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format == GL_BGR ? GL_RGB : GL_LUMINANCE, width, height, 0, format, GL_UNSIGNED_BYTE, bitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		is_dirty = false;
	}
}

void HWTexture::Bind(S32 texID, S32 tex_unit_id)
{
	Transfer();
	glActiveTexture(GL_TEXTURE0 + tex_unit_id);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texID, tex_unit_id);
}

S32 HWTexture::GetWidth() const
{
	return width;
}

S32 HWTexture::GetHeight() const
{
	return height;
}

S32 HWTexture::GetDepth() const
{
	return depth;
}

U8* HWTexture::GetBitmap()
{
	is_dirty = true;
	return bitmap;
}