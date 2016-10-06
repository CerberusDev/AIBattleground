// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "BTBase.h"

BTBase::BTBase()
{
	Root = new BTSelector(std::vector<BTNode*> {
		new BTBDecorator_Recovering(
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_FullHealth(
					new BTTask_SetBlackboardBRecoveringValue(false)),
				new BTBDecorator_HealZoneReached(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(),
						new BTTask_Idle()})),
				new BTTask_RetreatToHealZone()})),
		new BTBDecorator_NearestEnemySet( 
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_LowHealth(
					new BTTask_SetBlackboardBRecoveringValue(true)),
				new BTBDecorator_EnemyInRange(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(),
						new BTTask_Fight()})),
				new BTTask_GoTowardsNearestEnemy()})),
		new BTSequence(std::vector<BTNode*> {
			new BTTask_StopMovement(),
			new BTTask_Idle()})});
}

BTBase::~BTBase()
{

}

void BTBase::Update(AISystemBT* AISystem, Blackboard* argBlackboard, BTTask* TaskToUpdate)
{
	if (TaskToUpdate)
		TaskToUpdate->Update(AISystem, argBlackboard);
	else
		Root->Update(AISystem, argBlackboard);
}
