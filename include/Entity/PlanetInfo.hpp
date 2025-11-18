#pragma once

#include <SFML/Graphics.hpp>

struct PlanetInfo
{
    float Mass { 0 };
    float Radius { 0 };
    sf::Vector2f Position;
    sf::CircleShape Shape;

    PlanetInfo(float const mass, float const radius, sf::Vector2f const& position) :
        Mass{mass}, Radius{radius}, Position{position}
    {
        Shape.setRadius(Radius);
        Shape.setOrigin({Radius, Radius});
        Shape.setPosition(Position);
        Shape.setFillColor(sf::Color::White);
    }
};