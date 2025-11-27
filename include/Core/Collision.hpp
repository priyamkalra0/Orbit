#pragma once

#include "Entity/Player.hpp"
#include "Entity/Planet.hpp"

class Collision
{
public:
    static bool poll_collision(Player const& player);
    static bool intersects(Player const& player, PlanetInfo const& planet);
};
