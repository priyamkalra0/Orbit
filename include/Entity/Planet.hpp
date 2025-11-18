#pragma once

#include "PlanetInfo.hpp"
#include "Orbit.hpp"

class Planet
{
public:
    Planet(PlanetInfo const& info, Orbit const& orbit);
    Planet(sf::Vector2f const& position, float mass, float radius, float orbit_radius);

    void draw() const;

    sf::Vector2f const& get_position() const;

    void set_mass(float new_mass);
    float get_mass() const;

    Orbit& get_orbit() {return m_orbit;}
    Orbit const& get_orbit() const {return m_orbit;}

    PlanetInfo const& get_info() const {return m_info;}
    PlanetInfo& get_info() {return m_info;}

private:
    PlanetInfo m_info;
    Orbit m_orbit;
};