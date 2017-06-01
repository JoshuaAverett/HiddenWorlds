#include "pch.h"
#include "HWHeightMapRenderer.h"

HWHeightMapRenderer::HWHeightMapRenderer(HWHeightMap* map, HWTileSet* tiles)
{
	this->map = map;
	this->tiles = tiles;
	this->verts = new HWVertexBuffer(HWBufferType::Static);
    this->indices = new HWIndexBuffer(HWIndexType::UnsignedShort);
	this->selectedVerts = new HWVertexBuffer(HWBufferType::Dynamic);
	this->selectedIndices = new HWIndexBuffer(HWIndexType::UnsignedShort);
    this->model = new HWModel(*verts, *indices);
}

HWHeightMapRenderer::~HWHeightMapRenderer()
{
	delete verts;
	delete indices;
	delete selectedVerts;
	delete selectedIndices;
	delete model;
	delete form;
}

void HWHeightMapRenderer::Prepare(HWShaderProgram& program)
{
	S32 width = map->GetWidth();
	S32 height = map->GetHeight();

	S32 vertex_length = 8 * 5 * width * height;
	F32* vba = new F32[vertex_length];

	S32 index_length = 5 * 6 * width * height;
	U16* indices = new U16[index_length];

	S32 vi = 0;
	S32 ii = 0;

	for(S32 x = 0; x < width; ++x)
	{
		for(S32 y = 0; y < height; ++y)
		{
			HWHeightMapQuad quad = HWHeightMapQuad(x, y, map->GetTileHeight(x, y), map->GetTileType(x, y), tiles);
			quad.GetVerts(vba, indices, vi, ii);
			vi += 8 * 5;
			ii += 5 * 6;
		}
	}

	program.UseProgram();

	form = new HWVertexFormat(2);
    form->AddComponent(HWVertexComponent(program, "in_vertex", HWVertexComponentType::Single, 3));
    form->AddComponent(HWVertexComponent(program, "in_uv", HWVertexComponentType::Single, 2));
    verts->SetFormat(*form);
	selectedVerts->SetFormat(*form);

	model->AddVertexData(vba, sizeof(F32) * vi);
	model->AddIndexData(indices, sizeof(U16) * ii);
	model->Transfer();
	model->SetTexture(0, tiles->GetImage());
}

void HWHeightMapRenderer::Render(HWCamera* camera, HWShaderProgram* program)
{
	S32 width = map->GetWidth();
	S32 height = map->GetHeight();

	model->Render(camera, program);

	S32 vertex_length = 4 * 5 * width * height;
	F32* vba = new F32[vertex_length];

	S32 index_length = 6 * width * height;
	U16* indices = new U16[index_length];

	S32 vi = 0;
	S32 ii = 0;

	//TODO: move the path finder into the char so we don't need to update this every frame
	S32 char_count = chars.size();
	for(S32 i = 0; i < char_count; ++i)
	{
		HWTileCharacter ch = *chars[i];
		if(ch.IsSelected())
		{
			HWTilePathFinder finder = HWTilePathFinder(map, &ch);
			std::vector<glm::ivec2> tiles = finder.FindValidTiles();
			for(S32 i = 0; i < tiles.size(); ++i)
			{
				glm::ivec2 pos = tiles[i];
				U16 base = (U16)(vi / 5);
				indices[ii++] = base + 0;
				indices[ii++] = base + 1;
				indices[ii++] = base + 2;

				indices[ii++] = base + 2;
				indices[ii++] = base + 1;
				indices[ii++] = base + 3;

				const F32 tileFloatHeight = 1.f / 255.f;
				F32 z = map->GetTileHeight(pos.x, pos.y) / 255.f + tileFloatHeight;

				vba[vi++] = pos.x;
				vba[vi++] = pos.y;
				vba[vi++] = z;
				vba[vi++] = 0.f;
				vba[vi++] = 0.f;

				vba[vi++] = pos.x + 1;
				vba[vi++] = pos.y;
				vba[vi++] = z;
				vba[vi++] = 1.f;
				vba[vi++] = 0.f;

				vba[vi++] = pos.x;
				vba[vi++] = pos.y + 1;
				vba[vi++] = z;
				vba[vi++] = 0.f;
				vba[vi++] = 1.f;

				vba[vi++] = pos.x + 1;
				vba[vi++] = pos.y + 1;
				vba[vi++] = z;
				vba[vi++] = 1.f;
				vba[vi++] = 1.f;
			}
		}

		F32 height = ch.GetModel()->GetScale()[2];
		ch.GetModel()->SetPosition(glm::vec3(ch.GetX() + 0.5f, ch.GetY() + 0.5f, map->GetTileHeight(ch.GetX(), ch.GetY()) / 255.f + 0.35f));
		ch.GetModel()->Render(camera, program);
	}

	if(ii)
	{
		selectedVerts->SetDataDirect(vba, vi * sizeof(F32));
		selectedVerts->Transfer();
		selectedIndices->SetDataDirect(indices, ii * sizeof(U16));
		selectedIndices->Transfer();

		selectedVerts->Bind();
		selectedIndices->Bind();

		tiles->GetSelectionImage()->Bind(program->GetUniformID("texture"), 0);
		program->SetUniformValue("MVP", camera->GetMatrix());
		glDrawElements(GL_TRIANGLES, ii, (GLenum)selectedIndices->GetIndexType(), (void*)0);
	}

	delete[] vba;
	delete[] indices;
}

void HWHeightMapRenderer::AddCharacter(HWTileCharacter* person)
{
	chars.push_back(person);
}