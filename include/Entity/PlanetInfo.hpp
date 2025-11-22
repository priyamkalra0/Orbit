#pragma once

#include <SFML/Graphics.hpp>

struct PlanetInfo
{
    float mass { 0 };
    float radius { 0 };
    sf::Vector2f position;
    sf::CircleShape shape;
    sf::Color color;

    PlanetInfo(float const mass, float const radius, sf::Vector2f const& position, sf::Color const& color) :
        mass{mass}, radius{radius}, position{position}, color{color}
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius});
        shape.setPosition(position);
        shape.setFillColor(color);
    }
};