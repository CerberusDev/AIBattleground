// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class Blackboard
{
private:
	class Actor* Owner;
	float HP;
	float MaxHP;
	bool bHealthZoneDestReached;
	bool bEnemyInRange;
	bool bNearestEnemyIsSet;
	bool bSomeValueHasChanged;
	bool bBTRecovering;

public:
	Blackboard(Actor* argOwner);
	~Blackboard();

	Actor* GetOwner() const;

	void SetHP(float argHP);
	float GetHP() const;

	void SetMaxHP(float argMaxHP);
	float GetMaxHP() const;

	void SetBHealthZoneDestReached(bool argbHealthZoneDestReached);
	bool GetBHealthZoneDestReached() const;

	void SetBEnemyInRange(bool argbEnemyInRange);
	bool GetBEnemyInRange() const;

	void SetBNearestEnemyIsSet(bool argbNearestEnemyIsSet);
	bool GetBNearestEnemyIsSet() const;

	void SetBBTRecovering(bool argbBTRecovering);
	bool GetBBTRecovering() const;

	bool SomeValueHasChanged();
};
