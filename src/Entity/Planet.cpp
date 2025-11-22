#include "Entity/Planet.hpp"
#include "Graphics/Window.hpp"
#include "Math/Vector2.hpp"

Planet::Planet(PlanetInfo const& info, Orbit const& orbit)
    : m_info{info}, m_orbit{orbit} {}

Planet::Planet(
    sf::Vector2f const& position, float const mass, float const radius, sf::Color const& color,
    float const orbit_radius, sf::Color const& orbit_color
    ) : m_info{mass, radius, position, color},
        m_orbit{m_info, orbit_radius, orbit_color} {}

void Planet::draw() const
{
    m_orbit.draw();
    Window.draw(m_info.shape);
}

sf::Vector2f const& Planet::get_position() const {return m_info.position;}

float Planet::get_mass() const {return m_info.mass;}

void Planet::set_mass(float const new_mass)
{
    m_info.mass = new_mass;
}
