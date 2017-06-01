#pragma once

#include "HWTexture.h"

#include "../glm/glm.hpp"

#include <vector>

class HWTextureAtlas
{
public:
	friend class HWFont;
	HWTextureAtlas(S32 width, S32 height, S32 depth);
	~HWTextureAtlas();

	glm::ivec4 GetRegion(S32 width, S32 height);
	void SetRegion(S32 x, S32 y, S32 width, S32 height, const U8* data, S32 stride);

	void Bind(GLuint texID);

	S32 GetWidth() const;
	S32 GetHeight() const;
	S32 GetDepth() const;

private:
	std::vector<glm::ivec3> nodes;

	HWTexture* texture;

	S32 Fit(S32 index, S32 width, S32 height);
	void Merge();
};

