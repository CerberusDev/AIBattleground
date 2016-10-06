// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <vector>

#include "AISystemBase.h"

class AISystemBT : public AISystemBase
{
private:
	class BTBase* BehaviorTreeData;
	struct BTTask* PendingTask;
	class Blackboard* MyBlackboard;

public:
	AISystemBT(BTBase* argBehaviorTreeData, class Blackboard* argBlackboard);
	~AISystemBT();

	void UpdatePendingTask(BTTask* NewPendingTask);
	void Update();
};
