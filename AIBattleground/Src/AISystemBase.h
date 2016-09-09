// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class AISystemBase
{
protected:
	class Actor* Owner;
	class Blackboard* Blackboard;

public:
	AISystemBase(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemBase();

	virtual void Update() = 0;
};
