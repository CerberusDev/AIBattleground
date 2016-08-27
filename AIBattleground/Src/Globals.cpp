// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Globals.h"

void NormalizeVector2f(sf::Vector2f& InVector)
{
	float Length = std::sqrt(std::pow(InVector.x, 2) + std::pow(InVector.y, 2));

	if (Length != 0.0f)
	{
		InVector.x /= Length;
		InVector.y /= Length;
	}
}

float GetSquaredDist(const sf::Vector2f& VectorA, const sf::Vector2f& VectorB)
{
	return std::pow((VectorA.x - VectorB.x) + (VectorA.y - VectorB.y), 2);
}
