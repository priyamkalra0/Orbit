#pragma once

#include "PlanetInfo.hpp"

class Planet
{
public:
    explicit Planet(PlanetInfo const& info);
    Planet(sf::Vector2f const& position, float mass, float radius);
    void draw() const;

    sf::Vector2f const& get_position() const;

    void set_mass(float new_mass);
    float get_mass() const;

    PlanetInfo const& get_info() const {return m_info;}
    PlanetInfo& get_info() {return m_info;}

private:
    PlanetInfo m_info;
};