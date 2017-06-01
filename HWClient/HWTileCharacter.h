#pragma once

#include "../HWLib/HWHeightMap.h"
#include "HWModel.h"

class HWTileCharacter
{
public:
	HWTileCharacter(HWModel* model, S32 x, S32 y, S32 range, S32 jump_height, S32 fall_height);
	~HWTileCharacter();

	S32 GetX() const;
	S32 GetY() const;
	bool IsSelected() const;
	S32 GetRange() const;
	S32 GetJumpHeight() const;
	S32 GetFallHeight() const;
	HWModel* GetModel() const;

	void Select(bool selected);

	// Base stats
	S32 GetStrength() const;
	S32 GetConstution() const;
	S32 GetAgility() const;
	S32 GetIntelligence() const;

	void SetStrength(S32 strength);
	void SetConstution(S32 constution);
	void SetAgility(S32 agility);
	void SetIntelligence(S32 intelligence);

	std::string GetName() const;
	void SetName(std::string name);

	// Dynamic generated stats
	S32 GetMaxHealth() const;
	S32 GetMaxMana() const;

	S32 GetHealth() const;
	S32 GetMana() const;

	void SetHealth(S32 health);
	void SetMana(S32 mana);

	// Static generated stats
	F32 GetInitiative() const;
	F32 GetTurnTime() const;

private:
	S32 x, y;
	S32 range;
	S32 jump_height;
	S32 fall_height;
	bool selected;
	HWModel* model;

	std::string name;
	S32 strength, constution, agility, intelligence;
	S32 health, mana;
};

