#include "pch.h"
#include "HWTileCharacter.h"

HWTileCharacter::HWTileCharacter(HWModel* model, S32 x, S32 y, S32 range, S32 jump_height, S32 fall_height)
{
	this->model = model;
	this->x = x;
	this->y = y;
	this->range = range;
	this->jump_height = jump_height;
	this->fall_height = fall_height;
	this->selected = false;
}

HWTileCharacter::~HWTileCharacter()
{
}

S32 HWTileCharacter::GetX() const
{
	return x;
}

S32 HWTileCharacter::GetY() const
{
	return y;
}

bool HWTileCharacter::IsSelected() const
{
	return selected;
}

S32 HWTileCharacter::GetRange() const
{
	return range;
}

S32 HWTileCharacter::GetJumpHeight() const
{
	return jump_height;
}

S32 HWTileCharacter::GetFallHeight() const
{
	return fall_height;
}

HWModel* HWTileCharacter::GetModel() const
{
	return model;
}

void HWTileCharacter::Select(bool selected)
{
	this->selected = selected;
}

S32 HWTileCharacter::GetStrength() const
{
	return strength;
}

S32 HWTileCharacter::GetConstution() const
{
	return constution;
}

S32 HWTileCharacter::GetAgility() const
{
	return agility;
}

S32 HWTileCharacter::GetIntelligence() const
{
	return intelligence;
}

void HWTileCharacter::SetStrength(S32 strength)
{
	if(strength >= 10 && strength <= 25)
	{
		this->strength = strength;
	}
}

void HWTileCharacter::SetConstution(S32 constution)
{
	if(constution >= 10 && constution <= 25)
	{
		this->constution = constution;
	}
}

void HWTileCharacter::SetAgility(S32 agility)
{
	if(agility >= 10 && agility <= 25)
	{
		this->agility = agility;
	}
}

void HWTileCharacter::SetIntelligence(S32 intelligence)
{
	if(intelligence >= 10 && intelligence <= 25)
	{
		this->intelligence = intelligence;
	}
}

S32 HWTileCharacter::GetMaxHealth() const
{
	return (S32)((GetConstution() * 4.0f) + (GetStrength() * 2.4f) + 25.0f);
}

S32 HWTileCharacter::GetMaxMana() const
{
	return (S32)((GetConstution() * 2.0f - 20.0f) + (GetIntelligence() * 3.4f) - 10.0f);
}

S32 HWTileCharacter::GetHealth() const
{
	return health;
}

S32 HWTileCharacter::GetMana() const
{
	return mana;
}

void HWTileCharacter::SetHealth(S32 health)
{
	if(health >= 0 && health <= GetMaxHealth())
	{
		this->health = health;
	}
}

void HWTileCharacter::SetMana(S32 mana)
{
	if(mana >= 0 && mana <= GetMaxMana())
	{
		this->mana = mana;
	}
}

F32 HWTileCharacter::GetInitiative() const
{
	return (GetIntelligence() * 2.0f) + (GetAgility() * 1.5f) - 30.0f;
}

F32 HWTileCharacter::GetTurnTime() const
{
	return ((GetAgility() * 0.3f) + (GetIntelligence() * 0.1f)) * 0.25f + 4.0f;
}

std::string HWTileCharacter::GetName() const
{
	return name;
}

void HWTileCharacter::SetName(std::string name)
{
	this->name = name;
}
