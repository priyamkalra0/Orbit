#include "Graphics/Window.hpp"
#include "Entity/Orbit.hpp"

Orbit::Orbit(sf::Vector2f const& position, float const radius)
    : m_state{true}, m_radius{radius}
{
    constexpr sf::Color ring_color{ 255, 255, 255, 50 }; // Semi-transparent white
    constexpr int ring_count{ 15 };

    m_rings.resize(ring_count);

    int n { 1 };
    for (auto& ring : m_rings)
    {
        // Use a power function to bunch rings closer to the planet
        float const ratio{ static_cast<float>(n) / static_cast<float>(ring_count) };
        float current_radius{ m_radius * std::pow(ratio, 2.5f) };

        ring.setRadius(current_radius);
        ring.setOrigin({current_radius, current_radius});
        ring.setPosition(position);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(ring_color);
        ring.setOutlineThickness(2.0f);

        ++n;
    }
}

void Orbit::draw() const
{
    for (auto const& ring : m_rings)
        Window.draw(ring);
}
