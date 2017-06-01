#pragma once

#include "HWGL.h"

#include <string>

class HWTexture
{
public:
	HWTexture();
	HWTexture(S32 width, S32 height, S32 depth);
	HWTexture(const HWTexture&);
	~HWTexture();

	bool Read(const std::string& path);
	void FromPixels(const U8* data, S32 size, S32 width, S32 height, GLenum format);
	void Transfer();
	void Bind(S32 texID, S32 tex_unit_id);

	S32 GetWidth() const;
	S32 GetHeight() const;
	U8* GetBitmap();
	S32 GetDepth() const;

private:
	GLuint texture;
	GLenum format;
	U8* bitmap;
	S32 width, height, depth;
	bool is_dirty;
};

