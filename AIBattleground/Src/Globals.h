// ----------------------------------------------------------------------------
// ------------- AI Battleground, Copyright(C) Maciej Pryc, 2016 --------------
// ----------------------------------------------------------------------------

#pragma once

#include <SFML\Graphics.hpp>

enum class EDirection { NONE, UP, RIGHT, DOWN, LEFT };

enum class ETeam { TEAM_A, TEAM_B };

float Clamp(float Value, float Min, float Max);
void NormalizeVector2f(sf::Vector2f& InVector);
void ClampVector2f(sf::Vector2f& InVector, const sf::FloatRect& ClampRect);
float GetSquaredDist(const sf::Vector2f& VectorA, const sf::Vector2f& VectorB);
float GetLength(const sf::Vector2f& InVector);
float GetRandomFloat(float Max = 1.0f);
sf::Vector2f Abs(const sf::Vector2f& InVector);
