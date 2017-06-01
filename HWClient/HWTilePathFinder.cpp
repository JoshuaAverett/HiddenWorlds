#include "pch.h"
#include "HWTilePathFinder.h"

HWTilePathFinder::HWTilePathFinder(HWHeightMap* map, HWTileCharacter* ch)
{
	this->map = map;
	this->ch = ch;
}

bool HWTilePathFinder::CanReach(glm::ivec2 start, glm::ivec2 end)
{
	if(end.x < 0 || end.x > map->GetWidth()) return false;
	if(end.y < 0 || end.y > map->GetHeight()) return false;
	S32 delta = map->GetTileHeight(end.x, end.y) - map->GetTileHeight(start.x, start.y);
	
	if(delta > 0)
	{
		if(delta > ch->GetJumpHeight()) return false;
		return true;
	}
	return -delta <= ch->GetFallHeight();
}

std::vector<glm::ivec2> HWTilePathFinder::FindNehibors(glm::ivec2 tile)
{
	std::vector<glm::ivec2> result = std::vector<glm::ivec2>();

	if(CanReach(tile, tile + glm::ivec2( 1,  0))) result.push_back(tile + glm::ivec2( 1,  0));
	if(CanReach(tile, tile + glm::ivec2(-1,  0))) result.push_back(tile + glm::ivec2(-1,  0));
	if(CanReach(tile, tile + glm::ivec2( 0,  1))) result.push_back(tile + glm::ivec2( 0,  1));
	if(CanReach(tile, tile + glm::ivec2( 0, -1))) result.push_back(tile + glm::ivec2( 0, -1));

	return result;
}

std::vector<glm::ivec2> HWTilePathFinder::FindValidTiles()
{
	std::vector<glm::ivec3> open = std::vector<glm::ivec3>();
	std::vector<glm::ivec2> closed = std::vector<glm::ivec2>();

	open.push_back(glm::ivec3(ch->GetX(), ch->GetY(), ch->GetRange()));

	while(!open.empty())
	{
		glm::ivec3 n = open.back();
		open.pop_back();
		closed.push_back(glm::ivec2(n.x, n.y));

		if(n.z != 0)
		{
			std::vector<glm::ivec2> nehibors = FindNehibors(glm::ivec2(n.x, n.y));
			for(S32 i = 0; i < nehibors.size(); ++i)
			{
				glm::ivec2 pos = nehibors[i];
				if(!(std::find(closed.begin(), closed.end(), pos) != closed.end()))
				{
					open.push_back(glm::ivec3(pos.x, pos.y, n.z - 1));
				}
			}
		}
	}

	return closed;
}