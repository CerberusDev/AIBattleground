// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "AISystemFSM.h"
#include "Actor.h"
#include "Blackboard.h"

AISystemFSM::AISystemFSM(class Actor* argOwner, class Blackboard* argBlackboard) :
AISystemBase(argOwner, argBlackboard), State(EState::IDLE)
{

}

AISystemFSM::~AISystemFSM()
{

}

void AISystemFSM::Update()
{
	if (Blackboard->SomeValueHasChanged())
	{
		EState NewState = MoveToNewStateIfNeeded();

		if (State != NewState)
		{
			StateEnd();
			State = NewState;
			StateStart();
		}
	}

	StateUpdate();
}

AISystemFSM::EState AISystemFSM::MoveToNewStateIfNeeded()
{
	EState NewState = State;

	switch (State)
	{
	case EState::IDLE:
		if (Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::MOVE_TOWARDS_ENEMY;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		if (!Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::IDLE;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		if (Blackboard->GetBEnemyInRange())
			NewState = EState::FIGHT;
		break;
	case EState::FIGHT:
		if (!Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::IDLE;
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		if (!Blackboard->GetBEnemyInRange())
			NewState = EState::MOVE_TOWARDS_ENEMY;
		break;
	case EState::RETREAT:
		if (Blackboard->GetHP() == Blackboard->GetMaxHP())
			NewState = EState::IDLE;
		if (Blackboard->GetBHealthZoneDestReached())
			NewState = EState::RECOVER;
		break;
	case EState::RECOVER:
		if (Blackboard->GetHP() == Blackboard->GetMaxHP())
			NewState = EState::IDLE;
		break;
	}

	return NewState;
}

void AISystemFSM::StateStart()
{
	switch (State)
	{
	case EState::IDLE:
		Owner->StopMovement();
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		;
		break;
	case EState::FIGHT:
		Owner->StopMovement();
		break;
	case EState::RETREAT:
		;
		break;
	case EState::RECOVER:
		Owner->StopMovement();
		break;
	}
}

void AISystemFSM::StateEnd()
{
	switch (State)
	{
	case EState::IDLE:
		;
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		;
		break;
	case EState::FIGHT:
		;
		break;
	case EState::RETREAT:
		;
		break;
	case EState::RECOVER:
		;
		break;
	}
}

void AISystemFSM::StateUpdate()
{
	switch (State)
	{
	case EState::IDLE:
		;
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		Owner->GoTowardsNearestEnemy();
		break;
	case EState::FIGHT:
		Owner->TryToShoot();
		break;
	case EState::RETREAT:
		Owner->RetreatToHealZone();
		break;
	case EState::RECOVER:
		;
		break;
	}
}
