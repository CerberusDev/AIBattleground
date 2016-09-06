// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Blackboard.h"

Blackboard::Blackboard() :
HP(0.0f), MaxHP(0.0f), bHealthZoneDestReached(false), bEnemyInRange(false), NearestEnemy(nullptr)
{

}

Blackboard::~Blackboard()
{

}

void Blackboard::SetHP(float argHP)
{
	HP = argHP;
}
float Blackboard::GetHP() const
{
	return HP;
}

void Blackboard::SetMaxHP(float argMaxHP)
{
	MaxHP = argMaxHP;
}

float Blackboard::GetMaxHP() const
{
	return MaxHP;
}

void Blackboard::SetBHealthZoneDestReached(bool argbHealthZoneDestReached)
{
	bHealthZoneDestReached = argbHealthZoneDestReached;
}

bool Blackboard::GetBHealthZoneDestReached() const
{
	return bHealthZoneDestReached;
}

void Blackboard::SetBEnemyInRange(bool argbEnemyInRange)
{
	bEnemyInRange = argbEnemyInRange;
}

bool Blackboard::GetBEnemyInRange() const
{
	return bEnemyInRange;
}

void Blackboard::SetNearestEnemy(Actor* argNearestEnemy)
{
	NearestEnemy = argNearestEnemy;
}

Actor* Blackboard::GetNearestEnemy() const
{
	return NearestEnemy;
}
