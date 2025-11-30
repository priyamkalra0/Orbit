#pragma once

#include "Entity/Player.hpp"
#include "Entity/Planet.hpp"

class Collision
{
public:
    static bool poll_collision(sf::Shape const& shape);
    static bool intersects(PlanetInfo const& planet, sf::Shape const& shape);
};
