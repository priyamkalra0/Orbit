#include "Entity/Planet.hpp"
#include "Graphics/Window.hpp"

Planet::Planet(PlanetInfo const& info, Orbit const& orbit)
    : m_info{info}, m_orbit{orbit} {}

Planet::Planet(sf::Vector2f const& position, float const mass, float const radius, float const orbit_radius) :
    m_info{mass, radius, position}, m_orbit{m_info, orbit_radius} {}

void Planet::draw() const
{
    Window.draw(m_info.shape);
    m_orbit.draw();
}

sf::Vector2f const& Planet::get_position() const {return m_info.position;}

float Planet::get_mass() const {return m_info.mass;}

void Planet::set_mass(float const new_mass)
{
    m_info.mass = new_mass;
}
