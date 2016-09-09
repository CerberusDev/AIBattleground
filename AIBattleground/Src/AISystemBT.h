// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "AISystemBase.h"

class AISystemBT : public AISystemBase
{
public:
	AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemBT();

	void Update();
};
