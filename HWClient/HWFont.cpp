#include "pch.h"
#include "HWFont.h"

F32 round(F32 in)
{
	return (in > 0.0f) ? floorf(in + 0.5) : ceilf(in - 0.5);
}

HWFont::HWFont(std::string font_file, F32 size, S32 width, S32 height)
{
	this->vertex_buffer = new HWVertexBuffer(HWBufferType::Static);
	this->vertex_format = new HWVertexFormat(3);
	this->index_buffer = new HWIndexBuffer(HWIndexType::UnsignedShort);
	this->model = new HWModel(*vertex_buffer, *index_buffer);

	
	this->vertex_shader = new HWShader(HWShaderType::VertexShader);
	this->fragment_shader = new HWShader(HWShaderType::FragmentShader);
	this->shader_program = new HWShaderProgram();

	this->atlas = new HWTextureAtlas(width, height, 24);
	
	model->SetScale(glm::vec3(0.002f));

	vertex_shader->ReadSource("textshader.vtx");
	fragment_shader->ReadSource("textshader.frag");

	shader_program->AddShader(*vertex_shader);
	shader_program->AddShader(*fragment_shader);
	shader_program->CompileAndLink();

	vertex_format->AddComponent(HWVertexComponent(*shader_program, "in_vertex", HWVertexComponentType::Single, 3));
	vertex_format->AddComponent(HWVertexComponent(*shader_program, "in_uv", HWVertexComponentType::Single, 2));
	vertex_format->AddComponent(HWVertexComponent(*shader_program, "in_color", HWVertexComponentType::Single, 4));
	vertex_buffer->SetFormat(*vertex_format);
	
	this->hinting = true;
	this->kerning = true;
	this->filtering = true;
	this->outline = HWFontOutlineType::None;

	this->size = size;

	// FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
    // FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
    this->lcd_weights[0] = 0x10;
    this->lcd_weights[1] = 0x40;
    this->lcd_weights[2] = 0x70;
    this->lcd_weights[3] = 0x40;
    this->lcd_weights[4] = 0x10;

	FT_Init_FreeType(&library);
	FT_New_Face(library, font_file.c_str(), 0, &face);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	FT_Set_Char_Size(face, (S32)(size * 64.0f), 0, 72 * 64, 72);

	FT_Matrix matrix = {
		(int)((1.0f / 64.0f) * 0x10000L),
        (int)((0.0f)         * 0x10000L),
        (int)((0.0f)         * 0x10000L),
        (int)((1.0f)         * 0x10000L)
	};

    FT_Set_Transform(face, &matrix, NULL);

	// 64 * 64 because of 26.6 encoding AND the transform matrix used
    // in texture_font_load_face (hres = 64)
    underline_position = round((underline_position * size) / (64.0f * 64.0f));
	if(underline_position > -2.0f)
    {
        underline_position = -2.0f;
    }

    underline_thickness = round(face->underline_thickness * size / (64.0f * 64.0f));
    if(underline_thickness < 1.0f)
    {
        underline_thickness = 1.0f;
    }

	FT_Size_Metrics metrics = face->size->metrics;
	ascender = (metrics.ascender >> 6) / 100.0f;
	descender = (metrics.descender >> 6) / 100.0f;
	height = (metrics.height >> 6) / 100.0f;
	linegap = height - ascender + descender;

    glm::ivec4 region = atlas->GetRegion(5, 5);

    static unsigned char data[4*4*3] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	};

    atlas->SetRegion(region.x, region.y, 4, 4, data, 0);

	HWFontMetric glyph;
    glyph.width     = 0;
    glyph.height    = 0;
    glyph.offset_x  = 0;
    glyph.offset_y  = 0;
    glyph.advance_x = 0.0f;
    glyph.advance_y = 0.0f;

	glyph.outline = HWFontOutlineType::None;
    glyph.outline_thickness = 0.0f;
	glyph.charcode = (wchar_t)(-1);
	
    glyph.s0 = (region.x + 2) / (F32)width;
    glyph.t0 = (region.y + 2) / (F32)height;
    glyph.s1 = (region.x + 3) / (F32)width;
    glyph.t1 = (region.y + 3) / (F32)height;

    this->metrics[-1] = glyph;
}

HWFont::~HWFont()
{
	delete index_buffer;
	delete vertex_buffer;
	delete atlas;
}

void HWFont::AddVerts(std::vector<HWFontVertex>& vec, std::vector<U16>& idx, HWFontVertex v[4], U16 i[6])
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

S32 HWFont::LoadGlyphs(const std::string& chars)
{

    FT_Glyph ft_glyph;
    FT_GlyphSlot slot;
    FT_Bitmap ft_bitmap;

    glm::ivec4 region;
    size_t missed = 0;

    S32 width  = atlas->GetWidth();
    S32 height = atlas->GetHeight();
    S32 depth  = atlas->GetDepth() / 8;

    /* Load each glyph */
	for(S32 i=0; i < chars.length(); ++i)
    {
        FT_Int32 flags = 0;
        FT_UInt glyph_index = FT_Get_Char_Index(face, chars[i]);

		if(outline != HWFontOutlineType::None)
        {
            flags |= FT_LOAD_NO_BITMAP;
        }
        else
        {
            flags |= FT_LOAD_RENDER;
        }

        if(!hinting)
        {
            flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
        }
        else
        {
            flags |= FT_LOAD_FORCE_AUTOHINT;
        }


        if(depth == 3)
        {
            FT_Library_SetLcdFilter(library, FT_LCD_FILTER_LIGHT);
            flags |= FT_LOAD_TARGET_LCD;
            if(filtering)
            {
                FT_Library_SetLcdFilterWeights(library, lcd_weights);
            }
        }

		FT_Load_Glyph(face, glyph_index, flags);

        S32 ft_bitmap_width = 0;
        S32 ft_bitmap_rows = 0;
        S32 ft_bitmap_pitch = 0;
        S32 ft_glyph_top = 0;
        S32 ft_glyph_left = 0;

		if(outline == HWFontOutlineType::None)
        {
            slot            = face->glyph;
            ft_bitmap       = slot->bitmap;
            ft_bitmap_width = slot->bitmap.width;
            ft_bitmap_rows  = slot->bitmap.rows;
            ft_bitmap_pitch = slot->bitmap.pitch;
            ft_glyph_top    = slot->bitmap_top;
            ft_glyph_left   = slot->bitmap_left;
        }
        else
        {
            FT_Stroker stroker;
            FT_BitmapGlyph ft_bitmap_glyph;

            FT_Stroker_New(library, &stroker);
            FT_Stroker_Set(stroker, (S32)(outline_thickness * 64.f), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
			FT_Get_Glyph(face->glyph, &ft_glyph);

			switch(outline)
			{
			case HWFontOutlineType::Line:
				FT_Glyph_Stroke(&ft_glyph, stroker, 1);
				break;
			case HWFontOutlineType::Inner:
				FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
				break;
			case HWFontOutlineType::Outer:
				FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);
				break;
			}

          
            if(depth == 1)
            {
                FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            }
            else
            {
                FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_LCD, 0, 1);
            }

            ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
            ft_bitmap       = ft_bitmap_glyph->bitmap;
            ft_bitmap_width = ft_bitmap.width;
            ft_bitmap_rows  = ft_bitmap.rows;
            ft_bitmap_pitch = ft_bitmap.pitch;
            ft_glyph_top    = ft_bitmap_glyph->top;
            ft_glyph_left   = ft_bitmap_glyph->left;
            FT_Stroker_Done(stroker);
        }

        S32 w = ft_bitmap_width / depth + 1;
        S32 h = ft_bitmap_rows + 1;
		region = atlas->GetRegion(w, h);
        if (region.x < 0)
        {
            ++missed;
            continue;
        }

        --w;
		--h;
        S32 x = region.x;
        S32 y = region.y;
		atlas->SetRegion(x, y, w, h, ft_bitmap.buffer, ft_bitmap.pitch);

		HWFontMetric glyph;
        glyph.charcode = chars[i];
        glyph.width    = w;
        glyph.height   = h;
        glyph.outline  = outline;
        glyph.outline_thickness = outline_thickness;
        glyph.offset_x = ft_glyph_left;
        glyph.offset_y = ft_glyph_top;
        glyph.s0       = (F32)(x + 0.5f) / (F32)width;
        glyph.t0       = (F32)(y + 0.5f) / (F32)height;
        glyph.s1       = (x + glyph.width - 0.5f)  / (F32)width;
        glyph.t1       = (y + glyph.height - 0.5f) / (F32)height;

        // Discard hinting to get advance
        FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
        slot = face->glyph;
        glyph.advance_x = slot->advance.x / 64.0;
        glyph.advance_y = slot->advance.y / 64.0;

		metrics[chars[i]] = glyph;

		if(outline != HWFontOutlineType::None)
        {
            FT_Done_Glyph(ft_glyph);
        }
    }

    GenerateKerning();

    return missed;
}

void HWFont::GenerateKerning()
{
    /* For each glyph couple combination, check if kerning is necessary */
    /* Starts at index 1 since 0 is for the special background glyph */
    for(S32 i = 1; i < metrics.size(); ++i)
    {
		HWFontMetric glyph = metrics[i];
        S32 glyph_index = FT_Get_Char_Index(face, glyph.charcode);
		glyph.kerning.clear();

        for(S32 j = 1; j < metrics.size(); ++j)
        {
			HWFontMetric prev_glyph = metrics[j];
            S32 prev_index = FT_Get_Char_Index(face, prev_glyph.charcode);

			FT_Vector kerning;
            FT_Get_Kerning(face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning);

            if(kerning.x)
            {
				glyph.kerning[prev_glyph.charcode] = kerning.x / (float)(64.0f * 64.0f);
            }
        }
    }
}

void HWFont::AddText(const std::string& text, glm::vec4 color, glm::vec2 pen)
{
	F32 r = color.r, g = color.g, b = color.b, a = color.a;
	std::vector<HWFontVertex> verts;
	std::vector<U16> indices;

	for(S32 i = 0; i < text.length(); ++i)
    {
		if(metrics.find(text[i]) != metrics.end())
        {
			HWFontMetric glyph = metrics[text[i]];
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

	model->AddVertexData(&verts[0], verts.size() * sizeof(verts[0]));
	model->AddIndexData(&indices[0], indices.size() * sizeof(indices[0]));
	model->Transfer();
}

void HWFont::Render()
{
	shader_program->UseProgram();
	model->Bind();
	atlas->Bind(shader_program->GetUniformID("font_map"));
	glUniform1f(shader_program->GetUniformID("smoothness"), 0.0f);
	shader_program->SetUniformValue("MVP", model->GetMatrix());
	glDrawElements(GL_TRIANGLES, index_buffer->Count(), (GLenum)index_buffer->GetIndexType(), (void*)0);
}