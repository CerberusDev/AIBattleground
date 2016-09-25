// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "AISystemBase.h"

class AISystemFSM : public AISystemBase
{
public:
	enum class EState {IDLE, MOVE_TOWARDS_ENEMY, FIGHT, RETREAT, RECOVER};

private:
	EState State;

public:
	AISystemFSM(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemFSM();

	void Update();

	EState DetermineNewState();
	void StateStart();
	void StateEnd();
	void StateUpdate();
};
