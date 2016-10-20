// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Blackboard.h"

Blackboard::Blackboard(Actor* argOwner) :
Owner(argOwner), HP(0.0f), MaxHP(0.0f), bHealthZoneDestReached(false), bSomeValueHasChanged(false), bEnemyInRange(false), 
bNearestEnemyIsSet(false), bBTRecovering(false), bEnemyCapturePointAtLowHP(false), bEnemyCapturePointInRange(false), 
bMostEndangeredAlliedCapturePointIsSet(false), bNearAlliedCapturePoint(false), bBTGuardMode(false)
{

}

Blackboard::~Blackboard()
{

}

Actor* Blackboard::GetOwner() const
{
	return Owner;
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

void Blackboard::SetBNearestEnemyIsSet(bool argbNearestEnemyIsSet)
{
	if (bNearestEnemyIsSet != argbNearestEnemyIsSet)
	{
		bNearestEnemyIsSet = argbNearestEnemyIsSet;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBNearestEnemyIsSet() const
{
	return bNearestEnemyIsSet;
}

void Blackboard::SetBBTRecovering(bool argbBTRecovering)
{
	if (bBTRecovering != argbBTRecovering)
	{
		bBTRecovering = argbBTRecovering;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBBTRecovering() const
{
	return bBTRecovering;
}

void Blackboard::SetBEnemyCapturePointAtLowHP(bool argbEnemyCapturePointAtLowHP)
{
	if (bEnemyCapturePointAtLowHP != argbEnemyCapturePointAtLowHP)
	{
		bEnemyCapturePointAtLowHP = argbEnemyCapturePointAtLowHP;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBEnemyCapturePointAtLowHP() const
{
	return bEnemyCapturePointAtLowHP;
}

void Blackboard::SetBEnemyCapturePointInRange(bool argbEnemyCapturePointInRange)
{
	if (bEnemyCapturePointInRange != argbEnemyCapturePointInRange)
	{
		bEnemyCapturePointInRange = argbEnemyCapturePointInRange;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBEnemyCapturePointInRange() const
{
	return bEnemyCapturePointInRange;
}

void Blackboard::SetBMostEndangeredAlliedCapturePointIsSet(bool argbMostEndangeredAlliedCapturePointIsSet)
{
	if (bMostEndangeredAlliedCapturePointIsSet != argbMostEndangeredAlliedCapturePointIsSet)
	{
		bMostEndangeredAlliedCapturePointIsSet = argbMostEndangeredAlliedCapturePointIsSet;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBMostEndangeredAlliedCapturePointIsSet() const
{
	return bMostEndangeredAlliedCapturePointIsSet;
}

void Blackboard::SetBNearAlliedCapturePoint(bool argbNearAlliedCapturePoint)
{
	if (bNearAlliedCapturePoint != argbNearAlliedCapturePoint)
	{
		bNearAlliedCapturePoint = argbNearAlliedCapturePoint;
		bSomeValueHasChanged = true;
	}
}

bool Blackboard::GetBNearAlliedCapturePoint() const
{
	return bNearAlliedCapturePoint;
}

void Blackboard::SetBBTGuardMode(bool argbBTGuardMode)
{
	if (bNearAlliedCapturePoint != argbBTGuardMode)
	{
		bBTGuardMode = argbBTGuardMode;
		bBTGuardMode = true;
	}
}

bool Blackboard::GetBBTGuardMode() const
{
	return bBTGuardMode;
}

bool Blackboard::SomeValueHasChanged()
{
	bool bOldSomeValueHasChanged = bSomeValueHasChanged;
	bSomeValueHasChanged = false;
	return bOldSomeValueHasChanged;
}
