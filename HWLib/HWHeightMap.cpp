#include "Master.h"
#include "HWHeightMap.h"

HWHeightMap::HWHeightMap(S32 width, S32 height)
{
	this->width = width;
	this->height = height;
	this->data = new U16[width * height];
}

HWHeightMap::~HWHeightMap()
{
}

U8 HWHeightMap::GetTileType(S32 x, S32 y) const
{
	return data[x + y * width] >> 8;
}

U8 HWHeightMap::GetTileHeight(S32 x, S32 y) const
{
	return data[x + y * width] & 0xFF;
}

S32 HWHeightMap::GetWidth() const
{
	return width;
}

S32 HWHeightMap::GetHeight() const
{
	return height;
}

void HWHeightMap::SetTileType(S32 x, S32 y, U8 type)
{
	data[x + y * width] &= 0xFF;
	data[x + y * width] |= type << 8;
}

void HWHeightMap::SetTileHeight(S32 x, S32 y, U8 height)
{
	data[x + y * width] &= 0xFF00;
	data[x + y * width] |= height;
}

void HWHeightMap::GetDataLength(S32* size) const
{
	*size = width * height;
}

void HWHeightMap::GetData(U16** data) const
{
	*data = this->data;
}

void HWHeightMap::SetData(U16* data)
{
	this->data = data;
}

void HWHeightMap::SetData(U8* tiles, U8* heights)
{
	for(S32 x = 0; x < width; ++x)
	{
		for(S32 y = 0; y < height; ++y)
		{
			data[x + y * width] = heights[x + y * width] | (tiles[x + y * width] << 8);
		}
	}
}


