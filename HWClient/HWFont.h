#pragma once

#include "HWTextureAtlas.h"
#include "HWVertexBuffer.h"
#include "HWIndexBuffer.h"
#include "HWModel.h"
#include "../HWLib/HWFile.h"

#include "../freetype/windows/freetype/freetype/config/ftheader.h"
#include "../freetype/windows/freetype/freetype/freetype.h"
#include "../freetype/windows/freetype/freetype/ftstroke.h"
#include "../freetype/windows/freetype/freetype/ftlcdfil.h"
#pragma comment(lib, "freetype.lib")

#include <unordered_map>
#include <math.h>

struct HWFontVertex
{
    F32 x, y, z;    // position
    F32 s, t;       // texture
    F32 r, g, b, a; // color
};

enum class HWFontOutlineType
{
	None = 0,
	Line = 1,
	Inner = 2,
	Outer = 3,
};

struct HWFontMetric
{
    wchar_t charcode;

    S32 width;
    S32 height;

    S32 offset_x;
    S32 offset_y;

    F32 advance_x;
    F32 advance_y;

    F32 s0;
    F32 t0;

    F32 s1;
    F32 t1;

	std::unordered_map<wchar_t, F32> kerning;

    HWFontOutlineType outline;
    F32 outline_thickness;
};

class HWFont
{
public:
	friend class HWText;
	HWFont(std::string font_file, F32 size, S32 width, S32 height);
	~HWFont();

	S32 LoadGlyphs(const std::string& chars);

	void AddText(const std::string& text, glm::vec4 color, glm::vec2 pen);

	void Render();

private:
	std::unordered_map<wchar_t, HWFontMetric> metrics;

	std::string font_file;

	F32 size;

	HWFontOutlineType outline;
	float outline_thickness;

    bool filtering;
    bool kerning;
	bool hinting;

    U8 lcd_weights[5];

    F32 height;

    F32 linegap;

    F32 ascender;
    F32 descender;

    F32 underline_position;
    F32 underline_thickness;

	FT_Library library;
	FT_Face face;

	HWTextureAtlas* atlas;

	HWVertexBuffer* vertex_buffer;
	HWVertexFormat* vertex_format;
	HWIndexBuffer* index_buffer;
	HWModel* model;

	HWShader* vertex_shader;
	HWShader* fragment_shader;
	HWShaderProgram* shader_program;

	void AddVerts(std::vector<HWFontVertex>& vec, std::vector<U16>& idx, HWFontVertex v[4], U16 i[6]);

	bool LoadFace();
	void GenerateKerning();
};