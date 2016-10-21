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
					new BTSequence(std::vector<BTNode*> {
						new BTTask_SetRetreating(false),
						new BTTask_SetBlackboardBHealthZoneDestReached(false),
						new BTTask_SetBlackboardBRecoveringValue(false)})),
				new BTBDecorator_HealZoneReached(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(),
						new BTTask_SetRetreating(false),
						new BTTask_Idle()})),
				new BTTask_RetreatToHealZone()})),
		new BTSequence(std::vector<BTNode*> {
			new BTDecorator_ForceSuccess(
				new BTBDecorator_MostEndangeredAlliedCapturePointIsSet(
					new BTDecorateor_InverseBlackbordConditionAbove(
						new BTTask_SetBlackboardBGuardModeValue(false)))),
			new BTSelector(std::vector<BTNode*> {
				new BTBDecorator_LowHealth(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_SetRetreating(true),
						new BTTask_SetBlackboardBRecoveringValue(true)})),
				new BTBDecorator_EnemyCapturePointInRange(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(),
						new BTTask_SetBlackboardBGuardModeValue(false),
						new BTTask_TryToShootToEnemyCapturePoint()})),
				new BTBDecorator_EnemyInRange(
					new BTSequence(std::vector<BTNode*> {
						new BTTask_StopMovement(),
						new BTTask_SetBlackboardBGuardModeValue(false),
						new BTTask_ShootToEnemy()})),
				new BTBDecorator_MostEndangeredAlliedCapturePointIsSet(
					new BTSelector(std::vector<BTNode*> {
						new BTBDecorator_NearAlliedCapturePoint(
							new BTDecorateor_InverseBlackbordConditionAbove(
								new BTBDecorator_GuardMode(
									new BTDecorateor_InverseBlackbordConditionAbove(
										new BTTask_GoTowardsAlliedCapturePoint())))),
						new BTBDecorator_NearestEnemySet(
							new BTSequence(std::vector<BTNode*> {
								new BTTask_SetBlackboardBGuardModeValue(true),
								new BTTask_GoTowardsNearestEnemy()})),
							new BTSequence(std::vector<BTNode*> {
								new BTTask_StopMovement(),
								new BTTask_Idle()})})),
				new BTBDecorator_EnemyCapturePointAtLowHP(
					new BTDecorateor_InverseBlackbordConditionAbove(
						new BTTask_GoTowardsEnemyCapturePoint())),
				new BTBDecorator_NearestEnemySet(
					new BTTask_GoTowardsNearestEnemy())})}),
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
