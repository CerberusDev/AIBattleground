// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

enum class Direction { NONE, UP, RIGHT, DOWN, LEFT };

enum class ETeam { TEAM_A, TEAM_B };

void NormalizeVector2f(sf::Vector2f& InVector);
float GetSquaredDist(const sf::Vector2f& VectorA, const sf::Vector2f& VectorB);
