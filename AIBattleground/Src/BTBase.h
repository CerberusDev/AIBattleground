// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include "Actor.h"
#include "Blackboard.h"
#include "AISystemBT.h"


enum class EStatus {SUCCESS, FAIL, IN_PROGRESS};

struct BTNode
{
	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard) = 0;
};

struct BTComposite : public BTNode
{
	std::vector<BTNode*> Children;

	BTComposite(std::vector<BTNode*> argChildren) : Children(argChildren) {};
	virtual ~BTComposite()
	{
		for (auto it = Children.begin(); it != Children.end(); ++it)
			delete *it;
	}
};

struct BTSelector : public BTComposite
{
	BTSelector(std::vector<BTNode*> argChildren) : BTComposite(argChildren) {};
	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		for (auto it = Children.begin(); it != Children.end(); ++it)
		{
			EStatus ChildStatus = (*it)->Update(argAISystem, argBlackboard);

			if (ChildStatus != EStatus::FAIL)
			{
				return ChildStatus;
			}
		}

		return EStatus::FAIL;
	}
};

struct BTSequence : public BTComposite
{
	BTSequence(std::vector<BTNode*> argChildren) : BTComposite(argChildren) {};
	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		for (auto it = Children.begin(); it != Children.end(); ++it)
		{
			EStatus ChildStatus = (*it)->Update(argAISystem, argBlackboard);

			if (ChildStatus != EStatus::SUCCESS)
			{
				return ChildStatus;
			}
		}

		return EStatus::SUCCESS;
	}
};

struct BTDecorator : public BTNode
{
	BTNode* Child;

	BTDecorator(BTNode* argChild) : Child(argChild) {};
	virtual ~BTDecorator()
	{
		delete Child;
	}
};

struct BTDecorator_ForceSuccess : public BTDecorator
{
	BTDecorator_ForceSuccess(BTNode* argChild) : BTDecorator(argChild) {};

	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		Child->Update(argAISystem, argBlackboard);
		return EStatus::SUCCESS;
	}
};

struct BTDecorateor_InverseBlackbordConditionAbove: public BTDecorator
{
	BTDecorateor_InverseBlackbordConditionAbove(BTNode* argChild) : BTDecorator(argChild) {};

	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		return Child->Update(argAISystem, argBlackboard);
	}
};

struct BTBlackboardDecorator : public BTDecorator
{
	BTBlackboardDecorator(BTNode* argChild) : BTDecorator(argChild) {};

	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		bool bConditionValue = IsConditionFulfilled(argBlackboard);

		if (dynamic_cast<BTDecorateor_InverseBlackbordConditionAbove*>(Child))
			bConditionValue = !bConditionValue;

		if (bConditionValue)
			return Child->Update(argAISystem, argBlackboard);
		else
			return EStatus::FAIL;
	}

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard) = 0;
};

struct BTBDecorator_EnemyInRange : public BTBlackboardDecorator
{
	BTBDecorator_EnemyInRange(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBEnemyInRange();
	}
};

struct BTBDecorator_NearestEnemySet : public BTBlackboardDecorator
{
	BTBDecorator_NearestEnemySet(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBNearestEnemyIsSet();
	}
};

struct BTBDecorator_LowHealth : public BTBlackboardDecorator
{
	BTBDecorator_LowHealth(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBLowHP();
	}
};

struct BTBDecorator_FullHealth : public BTBlackboardDecorator
{
	BTBDecorator_FullHealth(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBFullHP();
	}
};

struct BTBDecorator_HealZoneReached : public BTBlackboardDecorator
{
	BTBDecorator_HealZoneReached(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBHealthZoneDestReached();
	}
};

struct BTBDecorator_Recovering : public BTBlackboardDecorator
{
	BTBDecorator_Recovering(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBBTRecovering();
	}
};

struct BTBDecorator_EnemyCapturePointAtLowHP : public BTBlackboardDecorator
{
	BTBDecorator_EnemyCapturePointAtLowHP(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBEnemyCapturePointAtLowHP();
	}
};

struct BTBDecorator_EnemyCapturePointInRange : public BTBlackboardDecorator
{
	BTBDecorator_EnemyCapturePointInRange(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBEnemyCapturePointInRange();
	}
};

struct BTBDecorator_MostEndangeredAlliedCapturePointIsSet : public BTBlackboardDecorator
{
	BTBDecorator_MostEndangeredAlliedCapturePointIsSet(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBMostEndangeredAlliedCapturePointIsSet();
	}
};

struct BTBDecorator_NearAlliedCapturePoint : public BTBlackboardDecorator
{
	BTBDecorator_NearAlliedCapturePoint(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBNearAlliedCapturePoint();
	}
};

struct BTBDecorator_GuardMode : public BTBlackboardDecorator
{
	BTBDecorator_GuardMode(BTNode* argChild) : BTBlackboardDecorator(argChild) {};

	virtual bool IsConditionFulfilled(Blackboard* argBlackboard)
	{
		return argBlackboard->GetBBTGuardMode();
	}
};

struct BTTask : public BTNode
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard) = 0;
	virtual EStatus Update(AISystemBT* argAISystem, Blackboard* argBlackboard)
	{
		EStatus Result = InternalUpdate(argBlackboard);

		if (Result == EStatus::IN_PROGRESS)
			argAISystem->UpdatePendingTask(this);

		return Result;
	}
};

struct BTTask_StopMovement : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->StopMovement();
		return EStatus::SUCCESS;
	}
};

struct BTTask_GoTowardsNearestEnemy : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->GoTowardsNearestEnemy();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_ShootToEnemy : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->TryToShoot();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_RetreatToHealZone : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->RetreatToHealZone();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_GoTowardsEnemyCapturePoint : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->GoTowardsEnemyCapturePoint();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_GoTowardsAlliedCapturePoint : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->GoTowardsAlliedCapturePoint();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_TryToShootToEnemyCapturePoint : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->TryToShootToEnemyCapturePoint();
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_Idle : public BTTask
{
	virtual EStatus InternalUpdate(Blackboard*)
	{
		return EStatus::IN_PROGRESS;
	}
};

struct BTTask_SetBlackboardBRecoveringValue : public BTTask
{
	bool bValueToSet;

	BTTask_SetBlackboardBRecoveringValue(bool argBValueToSet) : bValueToSet(argBValueToSet) {};

	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->SetBBTRecovering(bValueToSet);
		return EStatus::SUCCESS;
	}
};

struct BTTask_SetBlackboardBGuardModeValue : public BTTask
{
	bool bValueToSet;

	BTTask_SetBlackboardBGuardModeValue(bool argBValueToSet) : bValueToSet(argBValueToSet) {};

	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->SetBBTGuardMode(bValueToSet);
		return EStatus::SUCCESS;
	}
};

struct BTTask_SetBlackboardBHealthZoneDestReached : public BTTask
{
	bool bValueToSet;

	BTTask_SetBlackboardBHealthZoneDestReached(bool argBValueToSet) : bValueToSet(argBValueToSet) {};

	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->SetBHealthZoneDestReached(bValueToSet);
		return EStatus::SUCCESS;
	}
};

struct BTTask_SetRetreating : public BTTask
{
	bool bValueToSet;

	BTTask_SetRetreating(bool argBValueToSet) : bValueToSet(argBValueToSet) {};

	virtual EStatus InternalUpdate(Blackboard* argBlackboard)
	{
		argBlackboard->GetOwner()->SetReatreating(bValueToSet);
		return EStatus::SUCCESS;
	}
};

class BTBase
{
private:
	BTNode* Root;

public:
	BTBase();
	~BTBase();

	void Update(AISystemBT* AISystem, Blackboard* argBlackboard, BTTask* TaskToUpdate = nullptr);
};
