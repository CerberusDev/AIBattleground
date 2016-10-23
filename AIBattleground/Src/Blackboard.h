// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class Blackboard
{
private:
	class Actor* Owner;
	bool bLowHP;
	bool bFullHP;
	bool bHealthZoneDestReached;
	bool bEnemyInRange;
	bool bNearestEnemyIsSet;
	bool bSomeValueHasChanged;
	bool bBTRecovering;
	bool bEnemyCapturePointAtLowHP;
	bool bEnemyCapturePointInRange;
	bool bMostEndangeredAlliedCapturePointIsSet;
	bool bNearAlliedCapturePoint;
	bool bBTGuardMode;

public:
	Blackboard(Actor* argOwner);
	~Blackboard();

	Actor* GetOwner() const;

	void SetBLowHP(bool argbLowHP);
	bool GetBLowHP() const;

	void SetBFullHP(bool argbFullHP);
	bool GetBFullHP() const;

	void SetBHealthZoneDestReached(bool argbHealthZoneDestReached);
	bool GetBHealthZoneDestReached() const;

	void SetBEnemyInRange(bool argbEnemyInRange);
	bool GetBEnemyInRange() const;

	void SetBNearestEnemyIsSet(bool argbNearestEnemyIsSet);
	bool GetBNearestEnemyIsSet() const;

	void SetBBTRecovering(bool argbBTRecovering);
	bool GetBBTRecovering() const;

	void SetBEnemyCapturePointAtLowHP(bool argbEnemyCapturePointAtLowHP);
	bool GetBEnemyCapturePointAtLowHP() const;

	void SetBEnemyCapturePointInRange(bool argbEnemyCapturePointInRange);
	bool GetBEnemyCapturePointInRange() const;

	void SetBMostEndangeredAlliedCapturePointIsSet(bool argbMostEndangeredAlliedCapturePointIsSet);
	bool GetBMostEndangeredAlliedCapturePointIsSet() const;

	void SetBNearAlliedCapturePoint(bool argbNearAlliedCapturePoint);
	bool GetBNearAlliedCapturePoint() const;

	void SetBBTGuardMode(bool argbBTGuardMode);
	bool GetBBTGuardMode() const;

	bool SomeValueHasChanged();
};
