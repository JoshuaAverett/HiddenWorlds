#pragma once

#include "Master.h"

class HWHeightMap
{
public:
	HWHeightMap(S32 width, S32 height);
	HWHeightMap(const HWHeightMap& copy);
	~HWHeightMap();

	U8 GetTileType(S32 x, S32 y) const;
	U8 GetTileHeight(S32 x, S32 y) const;

	S32 GetWidth() const;
	S32 GetHeight() const;

	void SetTileType(S32 x, S32 y, U8 type);
	void SetTileHeight(S32 x, S32 y, U8 height);
	
	void GetDataLength(S32* size) const;
	void GetData(U16** data) const;

	void SetData(U16* data);
	void SetData(U8* tiles, U8* height);

private:
	U16* data;
	
	S32 width;
	S32 height;
};
