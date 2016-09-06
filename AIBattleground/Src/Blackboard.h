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
	class Actor* NearestEnemy;

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

	void SetNearestEnemy(Actor* argNearestEnemy);
	Actor* GetNearestEnemy() const;
};
