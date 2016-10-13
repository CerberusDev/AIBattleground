// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "AISystemBase.h"

class AISystemFSM : public AISystemBase
{
public:
	enum class EState {IDLE, MOVE_TOWARDS_ENEMY, MOVE_TOWARDS_ENEMY_CAPTURE_POINT, SHOOTING_TO_ENEMY_ACTOR, SHOOTING_TO_ENEMY_CAPTURE_POINT, RETREAT, RECOVER};

private:
	EState State;

public:
	AISystemFSM(class Blackboard* argBlackboard);
	~AISystemFSM();

	void Update();

	EState DetermineNewState();
	void StateStart();
	void StateEnd();
	void StateUpdate();
};
