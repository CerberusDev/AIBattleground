// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "AISystemFSM.h"
#include "Actor.h"
#include "Blackboard.h"

AISystemFSM::AISystemFSM(class Blackboard* argBlackboard) :
AISystemBase(argBlackboard), State(EState::IDLE)
{

}

AISystemFSM::~AISystemFSM()
{

}

void AISystemFSM::Update()
{
	if (Blackboard->SomeValueHasChanged())
	{
		EState NewState = DetermineNewState();

		while (State != NewState)
		{
			StateEnd();
			State = NewState;
			StateStart();

			NewState = DetermineNewState();
		}
	}

	StateUpdate();
}

AISystemFSM::EState AISystemFSM::DetermineNewState()
{
	EState NewState = State;

	switch (State)
	{
	case EState::IDLE:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		else if (Blackboard->GetBEnemyCapturePointInRange())
			NewState = EState::SHOOTING_TO_ENEMY_CAPTURE_POINT;
		else if (Blackboard->GetBMostEndangeredAlliedCapturePointIsSet())
		{
			if (!Blackboard->GetBNearAlliedCapturePoint())
				NewState = EState::MOVE_TOWARDS_ALLIED_CAPTURE_POINT;
			else if (Blackboard->GetBNearestEnemyIsSet())
				NewState = EState::MOVE_TOWARDS_ATTACKING_ENEMY;
		}
		else if (!Blackboard->GetBEnemyCapturePointAtLowHP())
			NewState = EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT;
		else if (Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::MOVE_TOWARDS_ENEMY;
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		else if (Blackboard->GetBEnemyCapturePointInRange())
			NewState = EState::SHOOTING_TO_ENEMY_CAPTURE_POINT;
		else if (!Blackboard->GetBEnemyCapturePointAtLowHP())
			NewState = EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT;
		else if (Blackboard->GetBMostEndangeredAlliedCapturePointIsSet())
			NewState = EState::IDLE;
		else if (!Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::IDLE;
		break;
	case EState::MOVE_TOWARDS_ATTACKING_ENEMY:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		else if (!Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::IDLE;
		break;
	case EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyCapturePointAtLowHP())
			NewState = EState::IDLE;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		else if (Blackboard->GetBEnemyCapturePointInRange())
			NewState = EState::SHOOTING_TO_ENEMY_CAPTURE_POINT;
		else if (Blackboard->GetBMostEndangeredAlliedCapturePointIsSet())
			NewState = EState::IDLE;
		break;
	case EState::MOVE_TOWARDS_ALLIED_CAPTURE_POINT:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		else if (!Blackboard->GetBMostEndangeredAlliedCapturePointIsSet())
			NewState = EState::IDLE;
		else if (Blackboard->GetBNearAlliedCapturePoint())
			NewState = EState::IDLE;
		break;
	case EState::SHOOTING_TO_ENEMY_ACTOR:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (!Blackboard->GetBEnemyInRange())
			NewState = EState::IDLE;
		else if (!Blackboard->GetBNearestEnemyIsSet())
			NewState = EState::IDLE;
		break;
	case EState::SHOOTING_TO_ENEMY_CAPTURE_POINT:
		if (Blackboard->GetHP() < Blackboard->GetMaxHP() * 0.5f)
			NewState = EState::RETREAT;
		else if (Blackboard->GetBEnemyInRange())
			NewState = EState::SHOOTING_TO_ENEMY_ACTOR;
		break;
	case EState::RETREAT:
		if (Blackboard->GetBHealthZoneDestReached())
			NewState = EState::RECOVER;
		else if (Blackboard->GetHP() == Blackboard->GetMaxHP())
			NewState = EState::IDLE;
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
		Blackboard->GetOwner()->StopMovement();
		break;
	case EState::MOVE_TOWARDS_ENEMY:
		;
		break;
	case EState::MOVE_TOWARDS_ATTACKING_ENEMY:
		;
		break;
	case EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT:
		;
		break;
	case EState::MOVE_TOWARDS_ALLIED_CAPTURE_POINT:
		;
		break;
	case EState::SHOOTING_TO_ENEMY_ACTOR:
		Blackboard->GetOwner()->StopMovement();
		break;
	case EState::SHOOTING_TO_ENEMY_CAPTURE_POINT:
		Blackboard->GetOwner()->StopMovement();
		break;
	case EState::RETREAT:
		;
		break;
	case EState::RECOVER:
		Blackboard->GetOwner()->StopMovement();
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
	case EState::MOVE_TOWARDS_ATTACKING_ENEMY:
		;
		break;
	case EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT:
		;
		break;
	case EState::MOVE_TOWARDS_ALLIED_CAPTURE_POINT:
		;
		break;
	case EState::SHOOTING_TO_ENEMY_ACTOR:
		;
		break;
	case EState::SHOOTING_TO_ENEMY_CAPTURE_POINT:
		;
		break;
	case EState::RETREAT:
		;
		break;
	case EState::RECOVER:
		Blackboard->SetBHealthZoneDestReached(false);
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
		Blackboard->GetOwner()->GoTowardsNearestEnemy();
		break;
	case EState::MOVE_TOWARDS_ATTACKING_ENEMY:
		Blackboard->GetOwner()->GoTowardsNearestEnemy();
		break;
	case EState::MOVE_TOWARDS_ENEMY_CAPTURE_POINT:
		Blackboard->GetOwner()->GoTowardsEnemyCapturePoint();
		break;
	case EState::MOVE_TOWARDS_ALLIED_CAPTURE_POINT:
		Blackboard->GetOwner()->GoTowardsAlliedCapturePoint();
		break;
	case EState::SHOOTING_TO_ENEMY_ACTOR:
		Blackboard->GetOwner()->TryToShoot();
		break;
	case EState::SHOOTING_TO_ENEMY_CAPTURE_POINT:
		Blackboard->GetOwner()->TryToShootToEnemyCapturePoint();
		break;
	case EState::RETREAT:
		Blackboard->GetOwner()->RetreatToHealZone();
		break;
	case EState::RECOVER:
		;
		break;
	}
}
