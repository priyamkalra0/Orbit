#pragma once

#include "Graphics/Color.hpp"
#include "Math/Vector2.hpp"

struct PlanetInfo
{
    sf::Vector2f position;

    float mass;
    float radius;
    sf::Color color;

    PlanetInfo(float const mass, float const radius, sf::Vector2f const& position, sf::Color const& color)
        : position{position}, mass{mass}, radius{radius}, color{color} {}
};