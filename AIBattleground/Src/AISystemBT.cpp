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
	Root = new BTSelector(std::vector<BTNode*> {
		new BTBDecorator_NearestEnemySet(Blackboard, 
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_EnemyInRange(Blackboard,
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(Owner),
						new BTTask_Fight(Owner)})),
				new BTTask_GoTowardsNearestEnemy(Owner)})),
		new BTTask_StopMovement(Owner)});
}

AISystemBT::~AISystemBT()
{
	delete Root;
}

void AISystemBT::Update()
{
	Root->Update();
}
