#pragma once

#include <vector>
#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"

class Navigation
{
public:
    Navigation(std::vector<Planet>& planets, Player& player);

    /* Active Planet: Closest planet whose orbit is currently on */
    Planet& get_active_planet() const;

private:
    std::vector<Planet>& m_planets;
    Player& m_player;
};
