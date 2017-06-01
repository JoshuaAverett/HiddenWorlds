#include "pch.h"
#include "HWTileSet.h"

HWTileSet::HWTileSet(HWTexture* image, HWTexture* selectionImage, S32 width, S32 height)
{
	this->width = width;
	this->height = height;
	this->image = image;
	this->selectionImage = selectionImage;
}

HWTileSet::~HWTileSet()
{
}

S32 HWTileSet::GetWidth() const
{
	return width;
}

S32 HWTileSet::GetHeight() const
{
	return height;
}

HWTexture* HWTileSet::GetImage() const
{
	return image;
}

HWTexture* HWTileSet::GetSelectionImage() const
{
	return selectionImage;
}
