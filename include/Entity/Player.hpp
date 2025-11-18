#pragma once

#include <SFML/Graphics.hpp>
#include "PlanetInfo.hpp"

class Player
{
public:
    Player(sf::Vector2f const& position, sf::Vector2f const& initial_velocity);

    void update();
    void draw() const;

    void accelerate(sf::Vector2f const& force);

    sf::Vector2f const& get_position() const { return m_position; }
    sf::Vector2f get_velocity() const;

    sf::Vector2f get_distance_vec(PlanetInfo const& planet) const;
    float get_distance(PlanetInfo const& planet) const;
    float get_distance_squared(PlanetInfo const& planet) const;

    void set_velocity(sf::Vector2f const& new_velocity); /* NOTE: WILL NULL THE ACCELERATION */
    void set_position(sf::Vector2f const& position); /* NOTE: WILL NULL THE VELOCITY */

private:
    sf::Vector2f m_position;
    sf::Vector2f m_previous_position;
    sf::Vector2f m_acceleration;

    sf::ConvexShape m_shape;
};
