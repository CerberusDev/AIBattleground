// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "AISystemBT.h"
#include "Actor.h"
#include "Blackboard.h"
#include "BTBase.h"

AISystemBT::AISystemBT(BTBase* argBehaviorTreeData, class Blackboard* argBlackboard) :
AISystemBase(argBlackboard), BehaviorTreeData(argBehaviorTreeData), PendingTask(nullptr), MyBlackboard(argBlackboard)
{

}

AISystemBT::~AISystemBT()
{

}

void AISystemBT::UpdatePendingTask(BTTask* NewPendingTask)
{
	PendingTask = NewPendingTask;
}

void AISystemBT::Update()
{
	BehaviorTreeData->Update(this, MyBlackboard, Blackboard->SomeValueHasChanged() ? nullptr : PendingTask);
}
