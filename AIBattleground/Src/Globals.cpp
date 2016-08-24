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
