// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "AISystemBase.h"

AISystemBase::AISystemBase(class Actor* argOwner, class Blackboard* argBlackboard) :
Owner(argOwner), Blackboard(argBlackboard)
{

}

AISystemBase::~AISystemBase()
{

}
