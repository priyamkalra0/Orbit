#include "Entity/Planet.hpp"
#include "Graphics/Window.hpp"

Planet::Planet(PlanetInfo const& info)
    : m_info{info} {}

Planet::Planet(sf::Vector2f const& position, float const mass, float const radius) :
    m_info{mass, radius, position} {}

void Planet::draw() const
{
    Window.draw(m_info.Shape);
}

sf::Vector2f const& Planet::get_position() const {return m_info.Position;}

float Planet::get_mass() const {return m_info.Mass;}

void Planet::set_mass(float const new_mass)
{
    m_info.Mass = new_mass;
}
