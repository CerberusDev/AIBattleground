// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

class AISystemFSM
{
public:
	enum class EState {IDLE, MOVE_TOWARDS_ENEMY, FIGHT, RETREAT, RECOVER};

private:
	EState State;
	class Actor* Owner;
	class Blackboard* Blackboard;

public:
	AISystemFSM(class Actor* argOwner, class Blackboard* argBlackboard);
	~AISystemFSM();

	void Update();
};
