// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "AISystemBT.h"
#include "Actor.h"
#include "Blackboard.h"

AISystemBT::AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard) :
AISystemBase(argOwner, argBlackboard), Root(nullptr)
{
	BTSelector* FirstSelector = new BTSelector();
	Root = FirstSelector;
	BTTask_GoTowardsNearestEnemy* Task = new BTTask_GoTowardsNearestEnemy(Owner);
	BTBDecorator_NearestEnemySet* Dec = new BTBDecorator_NearestEnemySet(Blackboard, Task);
	FirstSelector->AddChild(Dec);
}

AISystemBT::~AISystemBT()
{
	delete Root;
}

void AISystemBT::Update()
{
	Root->Update();
}
