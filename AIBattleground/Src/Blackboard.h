// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class Blackboard
{
private:
	float HP;
	float MaxHP;
	bool bHealthZoneDestReached;
	bool bEnemyInRange;
	bool bNearestEnemyIsSet;
	bool bSomeValueHasChanged;

public:
	Blackboard();
	~Blackboard();

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

	bool SomeValueHasChanged();
};
