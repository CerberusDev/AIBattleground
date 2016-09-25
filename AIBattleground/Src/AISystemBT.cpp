// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include <iostream>

#include "AISystemBT.h"
#include "Actor.h"
#include "Blackboard.h"

AISystemBT::AISystemBT(class Actor* argOwner, class Blackboard* argBlackboard) :
AISystemBase(argOwner, argBlackboard), Root(nullptr), PendingTask(nullptr), BTStorage_Bool(false), bReevalutateTree(false)
{
	Root = new BTSelector(std::vector<BTNode*> {
		new BTDecorator_BTStorage_Bool(this,
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_FullHealth(Blackboard,
					new BTTask_SetBTStorage_Bool(this, false)),
				new BTBDecorator_HealZoneReached(Blackboard,
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(this, Owner),
						new BTTask_Idle(this)})),
				new BTTask_RetreatToHealZone(this, Owner)})),
		new BTBDecorator_NearestEnemySet(Blackboard, 
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_LowHealth(Blackboard,
					new BTTask_SetBTStorage_Bool(this, true)),
				new BTBDecorator_EnemyInRange(Blackboard,
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(this, Owner),
						new BTTask_Fight(this, Owner)})),
				new BTTask_GoTowardsNearestEnemy(this, Owner)})),
		new BTSequence(std::vector<BTNode*> {
			new BTTask_StopMovement(this, Owner),
			new BTTask_Idle(this)})});
}

AISystemBT::~AISystemBT()
{
	delete Root;
}

void AISystemBT::UpdatePendingTask(BTTask* NewPendingTask)
{
	PendingTask = NewPendingTask;
}

void AISystemBT::Update()
{
	if (Blackboard->SomeValueHasChanged() || PendingTask == nullptr || bReevalutateTree)
	{
		bReevalutateTree = false;
		Root->Update();
	}
	else
	{
		PendingTask->Update();
	}
}
