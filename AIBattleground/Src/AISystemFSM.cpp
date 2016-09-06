// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "AISystemFSM.h"
#include "Actor.h"
#include "Blackboard.h"

AISystemFSM::AISystemFSM(class Actor* argOwner, class Blackboard* argBlackboard) :
State(EState::IDLE), Owner(argOwner), Blackboard(argBlackboard)
{

}

AISystemFSM::~AISystemFSM()
{

}

void AISystemFSM::Update()
{
	switch (State)
	{
	case EState::IDLE:
		if (Blackboard->GetNearestEnemy())
			State = EState::MOVE_TOWARDS_ENEMY;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			State = EState::RETREAT;
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		if (Blackboard->GetNearestEnemy() == nullptr)
			State = EState::IDLE;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			State = EState::RETREAT;
		if (Blackboard->GetBEnemyInRange())
			State = EState::FIGHT;
		break;
	case EState::FIGHT:
		if (Blackboard->GetNearestEnemy() == nullptr)
			State = EState::IDLE;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			State = EState::RETREAT;
		if (!Blackboard->GetBEnemyInRange())
			State = EState::MOVE_TOWARDS_ENEMY;
		break;
	case EState::RETREAT:
		if (Blackboard->GetHP() == Blackboard->GetMaxHP())
			State = EState::IDLE;
		if (Blackboard->GetBHealthZoneDestReached())
			State = EState::RECOVER;
		break;
	case EState::RECOVER:
		if (Blackboard->GetHP() == Blackboard->GetMaxHP())
			State = EState::IDLE;
		break;
	}

	switch (State)
	{
	case EState::IDLE:
		Owner->StopMovement();
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		Owner->GoTowardsNearestEnemy();
		break;
	case EState::FIGHT:
		Owner->StopMovement();
		Owner->TryToShoot();
		break;
	case EState::RETREAT:
		Owner->RetreatToHealZone();
		break;
	case EState::RECOVER:
		Owner->StopMovement();
		break;
	}
}
