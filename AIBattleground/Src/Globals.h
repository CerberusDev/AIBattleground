#pragma once

#include <SFML\Graphics.hpp>

enum class Direction { NONE, UP, RIGHT, DOWN, LEFT };

void NormalizeVector2f(sf::Vector2f& InVector);
