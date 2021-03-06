// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#include "Globals.h"

float Clamp(float Value, float Min, float Max)
{
	return std::max(std::min(Value, Max), Min);
}

void NormalizeVector2f(sf::Vector2f& InVector)
{
	float Length = GetLength(InVector);

	if (Length != 0.0f)
	{
		InVector.x /= Length;
		InVector.y /= Length;
	}
}

void ClampVector2f(sf::Vector2f& InVector, const sf::FloatRect& ClampRect)
{
	InVector.x = Clamp(InVector.x, ClampRect.left, ClampRect.left + ClampRect.width);
	InVector.y = Clamp(InVector.y, ClampRect.top, ClampRect.top + ClampRect.height);
}

float GetSquaredDist(const sf::Vector2f& VectorA, const sf::Vector2f& VectorB)
{
	return std::pow((VectorA.x - VectorB.x), 2) + std::pow((VectorA.y - VectorB.y), 2);
}

float GetSquaredLength(const sf::Vector2f& InVector)
{
	return std::pow(InVector.x, 2) + std::pow(InVector.y, 2);
}

float GetLength(const sf::Vector2f& InVector)
{
	return std::sqrt(GetSquaredLength(InVector));
}

float GetRandomFloat(float Max)
{
	return (float)std::rand() / ((float)RAND_MAX / Max);
}

sf::Vector2f Abs(const sf::Vector2f& InVector)
{
	return sf::Vector2f(std::abs(InVector.x), std::abs(InVector.y));
}
