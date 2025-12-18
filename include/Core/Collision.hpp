#pragma once

#include "Entity/Planet.hpp"

class Collision
{
public:
    static bool with_any_planet(sf::Shape const& shape);
    static bool with_planet(PlanetInfo const& planet, sf::Shape const& shape);
};
