#pragma once

#include <SFML/Graphics.hpp>

struct PlanetInfo
{
    float mass { 0 };
    float radius { 0 };
    sf::Vector2f position;
    sf::CircleShape shape;

    PlanetInfo(float const mass, float const radius, sf::Vector2f const& position) :
        mass{mass}, radius{radius}, position{position}
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius});
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);
    }
};