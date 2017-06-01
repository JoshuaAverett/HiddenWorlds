#pragma once

#include "HWTexture.h"

class HWTileSet
{
public:
	HWTileSet(HWTexture* image, HWTexture* selectionImage, S32 width, S32 height);
	~HWTileSet();

	S32 GetWidth() const;
	S32 GetHeight() const;
	HWTexture* GetImage() const;
	HWTexture* GetSelectionImage() const;

private:
	HWTexture* image;
	HWTexture* selectionImage;
	S32 width;
	S32 height;
};

