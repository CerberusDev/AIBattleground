// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class AISystemBase
{
protected:
	class Blackboard* Blackboard;

public:
	AISystemBase(class Blackboard* argBlackboard);
	~AISystemBase();

	virtual void Update() = 0;
};
