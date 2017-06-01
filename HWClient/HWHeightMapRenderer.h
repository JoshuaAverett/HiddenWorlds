#pragma once

#include "../HWLib/HWHeightMap.h"
#include "HWGL.h"
#include "HWModel.h"
#include "HWTileSet.h"
#include "HWTileCharacter.h"
#include "HWTilePathFinder.h"
#include "HWCamera.h"

#include <vector>

class HWHeightMapQuad
{
public:
	HWHeightMapQuad(S32 x, S32 y, U8 height, U8 type, HWTileSet* tiles)
	{
		this->x = (F32)x;
		this->y = (F32)y;
		this->z = height / 255.f;
		this->type = type;
		this->tiles = tiles;
	}

	void GetVerts(F32* verts, U16* indices, S32 vi, S32 ii)
	{
		U16 base = (U16)(vi / 5);

		// top side
		indices[ii++] = base + 0;
		indices[ii++] = base + 1;
		indices[ii++] = base + 2;

		indices[ii++] = base + 2;
		indices[ii++] = base + 1;
		indices[ii++] = base + 3;

		// front side
		indices[ii++] = base + 0;
		indices[ii++] = base + 4;
		indices[ii++] = base + 1;

		indices[ii++] = base + 1;
		indices[ii++] = base + 4;
		indices[ii++] = base + 5;

		// right side
		indices[ii++] = base + 3;
		indices[ii++] = base + 1;
		indices[ii++] = base + 5;

		indices[ii++] = base + 3;
		indices[ii++] = base + 5;
		indices[ii++] = base + 7;

		// back side
		indices[ii++] = base + 2;
		indices[ii++] = base + 3;
		indices[ii++] = base + 7;

		indices[ii++] = base + 2;
		indices[ii++] = base + 7;
		indices[ii++] = base + 6;

		// back side
		indices[ii++] = base + 2;
		indices[ii++] = base + 4;
		indices[ii++] = base + 0;

		indices[ii++] = base + 4;
		indices[ii++] = base + 2;
		indices[ii++] = base + 6;

		F32 tileWidth = 1.f / (F32)tiles->GetWidth();
		F32 tileHeight = 1.f / (F32)tiles->GetHeight();

		S32 tileX = type % tiles->GetWidth();
		S32 tileY = type / tiles->GetWidth();

		// vert data
		verts[vi++] = x;
		verts[vi++] = y;
		verts[vi++] = z;
		verts[vi++] = tileX * tileWidth;
		verts[vi++] = tileY * tileHeight;

		verts[vi++] = x + 1.f;
		verts[vi++] = y;
		verts[vi++] = z;
		verts[vi++] = (tileX + 1) * tileWidth;
		verts[vi++] = tileY * tileHeight;

		verts[vi++] = x;
		verts[vi++] = y + 1.f;
		verts[vi++] = z;
		verts[vi++] = tileX * tileWidth;
		verts[vi++] = (tileY + 1) * tileHeight;

		verts[vi++] = x + 1.f;
		verts[vi++] = y + 1.f;
		verts[vi++] = z;
		verts[vi++] = (tileX + 1) * tileWidth;
		verts[vi++] = (tileY + 1) * tileHeight;


		verts[vi++] = x;
		verts[vi++] = y;
		verts[vi++] = z - 1.f;
		verts[vi++] = tileX * tileWidth;
		verts[vi++] = tileY * tileHeight;

		verts[vi++] = x + 1.f;
		verts[vi++] = y;
		verts[vi++] = z - 1.f;
		verts[vi++] = (tileX + 1) * tileWidth;
		verts[vi++] = tileY * tileHeight;

		verts[vi++] = x;
		verts[vi++] = y + 1.f;
		verts[vi++] = z - 1.f;
		verts[vi++] = tileX * tileWidth;
		verts[vi++] = (tileY + 1) * tileHeight;

		verts[vi++] = x + 1.f;
		verts[vi++] = y + 1.f;
		verts[vi++] = z - 1.f;
		verts[vi++] = (tileX + 1) * tileWidth;
		verts[vi++] = (tileY + 1) * tileHeight;
	}

private:
	F32 x, y, z;
	U8 type;
	HWTileSet* tiles;
};

class HWHeightMapRenderer
{
public:
	HWHeightMapRenderer(HWHeightMap* map, HWTileSet* tiles);
	~HWHeightMapRenderer();

	void Prepare(HWShaderProgram& program);
	void Render(HWCamera* camera, HWShaderProgram* program);

	void AddCharacter(HWTileCharacter* person);

private:
	HWHeightMap* map;

	HWModel* model;
	HWVertexBuffer* verts;
    HWIndexBuffer* indices;

	HWVertexBuffer* selectedVerts;
    HWIndexBuffer* selectedIndices;

	HWVertexFormat* form;
	HWTileSet* tiles;

	std::vector<HWTileCharacter*> chars;
};

