#pragma once

#include "HWTileCharacter.h"
#include "../HWLib/HWHeightMap.h"

#include <vector>
#include "../glm/glm.hpp"

class HWTilePathFinder
{
public:
	HWTilePathFinder::HWTilePathFinder(HWHeightMap* map, HWTileCharacter* ch);
	std::vector<glm::ivec2> FindNehibors(glm::ivec2 tile);
	std::vector<glm::ivec2> FindValidTiles();

private:
	HWTileCharacter* ch;
	HWHeightMap* map;

	bool CanReach(glm::ivec2 start, glm::ivec2 end);
};

