// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Blackboard.h"

Blackboard::Blackboard() :
HP(0.0f), MaxHP(0.0f), bHealthZoneDestReached(false), bSomeValueHasChanged(false), bEnemyInRange(false), NearestEnemy(nullptr)
{

}

Blackboard::~Blackboard()
{

}

void Blackboard::SetHP(float argHP)
{
	if (HP != argHP)
	{
		HP = argHP;
		bSomeValueHasChanged = true;
	}
}
float Blackboard::GetHP() const
{
	return HP;
}

void Blackboard::SetMaxHP(float argMaxHP)
{
	if (MaxHP != argMaxHP)
	{
		MaxHP = argMaxHP;
		bSomeValueHasChanged = true;
	}
}

float Blackboard::GetMaxHP() const
{
	return MaxHP;
}

void Blackboard::SetBHealthZoneDestReached(bool argbHealthZoneDestReached)
{
	if (bHealthZoneDestReached != argbHealthZoneDestReached)
	{
		bHealthZoneDestReached = argbHealthZoneDestReached;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBHealthZoneDestReached() const
{
	return bHealthZoneDestReached;
}

void Blackboard::SetBEnemyInRange(bool argbEnemyInRange)
{
	if (bEnemyInRange != argbEnemyInRange)
	{
		bEnemyInRange = argbEnemyInRange;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBEnemyInRange() const
{
	return bEnemyInRange;
}

void Blackboard::SetNearestEnemy(Actor* argNearestEnemy)
{
	if (NearestEnemy != argNearestEnemy)
	{
		NearestEnemy = argNearestEnemy;
		bSomeValueHasChanged = true;
	}
}

Actor* Blackboard::GetNearestEnemy() const
{
	return NearestEnemy;
}

bool Blackboard::SomeValueHasChanged()
{
	bool bOldSomeValueHasChanged = bSomeValueHasChanged;
	bSomeValueHasChanged = false;
	return bOldSomeValueHasChanged;
}
