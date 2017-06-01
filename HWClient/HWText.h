#pragma once

#include <string>
#include "../glm/glm.hpp"
#include "HWFont.h"

class HWText
{
public:
	HWText(HWFont* font, const std::string& text, glm::vec2 location, glm::vec4 color);
	~HWText();

	void Render();

	void SetText(const std::string& text);

private:
	HWFont* font;
	std::string text;
	glm::vec2 location;
	glm::vec4 color;

	bool is_dirty;

	void AddVerts(std::vector<HWFontVertex>& vec, std::vector<U16>& idx, HWFontVertex v[4], U16 i[6]);
};

