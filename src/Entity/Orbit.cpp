#include "Graphics/Window.hpp"
#include "Graphics/World.hpp"
#include "Entity/Orbit.hpp"
#include "Entity/Player.hpp"

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

        // force the last orbit a little bit further out
        // NOTE: WHY?
        // because it will help the player aim;
        // if they aim for orbit radius,
        // the gravity of the planet will
        // pull them in as they travel towards the planet;
        // fucking up their pathing.
        // but pushing this ring outwards will make
        // the player aim a little extra outwards to compensate.
        if (n == ring_count) current_radius += World.scale_distance(7.0f);

        ring.setRadius(current_radius);
        ring.setOrigin({current_radius, current_radius});
        ring.setPosition(position);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(ring_color);
        ring.setOutlineThickness(2.0f);

        ++n;
    }
}

sf::Vector2f Orbit::calculate_force(PlanetInfo const& planet, Player const& player) const
{
    if (!m_state) return {0.0f, 0.0f};

    static constexpr float G { 10.0f };

    sf::Vector2f const distance_vec = player.get_distance_vec(planet);
    float const distance = distance_vec.length();

    if (distance <= 1.0f) return {0.0f, 0.0f}; // Too close = massive force

    sf::Vector2f const direction { -distance_vec.normalized() };
    float const force_magnitude { (G * planet.Mass) / (distance * distance) };

    return direction * force_magnitude;
}

void Orbit::apply_force(PlanetInfo const& planet, Player& player) const
{
    player.accelerate(calculate_force(planet, player));
}

void Orbit::draw() const
{
    if (!m_state) return;
    for (auto const& ring : m_rings)
        Window.draw(ring);
}