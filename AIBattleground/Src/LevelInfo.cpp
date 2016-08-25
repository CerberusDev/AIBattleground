#include "LevelInfo.h"

LevelInfo::LevelInfo(const sf::FloatRect& LevelBoundaries) :
Boundaries(LevelBoundaries), RightBottomEdge(LevelBoundaries.left + LevelBoundaries.width, LevelBoundaries.top + LevelBoundaries.height)
{

}

LevelInfo::~LevelInfo()
{
}
