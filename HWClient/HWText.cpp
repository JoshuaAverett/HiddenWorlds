#include "pch.h"
#include "HWText.h"

HWText::HWText(HWFont* font, const std::string& text, glm::vec2 location, glm::vec4 color)
{
	this->font = font;
	this->text = text;
	this->location = location;
	this->color = color;

	is_dirty = true;
}

HWText::~HWText()
{
}

void HWText::Render()
{
	if(is_dirty)
	{
		F32 r = color.r, g = color.g, b = color.b, a = color.a;
		std::vector<HWFontVertex> verts;
		std::vector<U16> indices;

		glm::vec2 pen;

		for(S32 i = 0; i < text.length(); ++i)
		{
			if(font->metrics.find(text[i]) != font->metrics.end())
			{
				HWFontMetric glyph = font->metrics[text[i]];
				if(i > 0 && glyph.kerning.find(text[i - 1]) != glyph.kerning.end())
				{
					F32 kerning = glyph.kerning[text[i - 1]];
					pen.x += kerning;
				}

				int x0  = (int)(pen.x + glyph.offset_x);
				int y0  = (int)(pen.y + glyph.offset_y);

				int x1  = (int)(x0 + glyph.width);
				int y1  = (int)(y0 - glyph.height);

				float s0 = glyph.s0;
				float t0 = glyph.t0;

				float s1 = glyph.s1;
				float t1 = glyph.t1;

				U16 base = i * 4;

				U16 idx[6] = {
					base, base + 1, base + 2,
					base, base + 2, base + 3,
				};

				HWFontVertex v[4] = {
					{ x0,y0,0,  s0,t0, r,g,b,a },
					{ x0,y1,0,  s0,t1, r,g,b,a },
					{ x1,y1,0,  s1,t1, r,g,b,a },
					{ x1,y0,0,  s1,t0, r,g,b,a },
				};

				AddVerts(verts, indices, v, idx);

				pen.x += glyph.advance_x;
			}
		}
		
		//font->model->AddVertexData(&verts[0], verts.size() * sizeof(verts[0]));
		//font->model->AddIndexData(&indices[0], indices.size() * sizeof(indices[0]));
		font->vertex_buffer->SetDataDirect(&verts[0], verts.size() * sizeof(verts[0]));
		font->index_buffer->SetDataDirect(&indices[0], indices.size() * sizeof(indices[0]));
		font->model->Transfer();

		is_dirty = false;
	}

	font->shader_program->UseProgram();
	font->model->Bind();
	font->atlas->Bind(font->shader_program->GetUniformID("font_map"));
	glUniform1f(font->shader_program->GetUniformID("smoothness"), 0.0f);
	font->model->SetPosition(glm::vec3(location, 0.f));
	font->shader_program->SetUniformValue("MVP", font->model->GetMatrix());
	glDrawElements(GL_TRIANGLES, font->index_buffer->Count(), (GLenum)font->index_buffer->GetIndexType(), (void*)0);
}

void HWText::AddVerts(std::vector<HWFontVertex>& vec, std::vector<U16>& idx, HWFontVertex v[4], U16 i[6])
{
	vec.push_back(v[0]);
	vec.push_back(v[1]);
	vec.push_back(v[2]);
	vec.push_back(v[3]);

	idx.push_back(i[0]);
	idx.push_back(i[1]);
	idx.push_back(i[2]);
	idx.push_back(i[3]);
	idx.push_back(i[4]);
	idx.push_back(i[5]);
}

void HWText::SetText(const std::string& text)
{
	if(this->text != text)
	{
		this->text = text;
		is_dirty = true;
	}
}