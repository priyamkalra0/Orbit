#include "Entity/Orbit.hpp"
#include "Entity/Player.hpp"
#include "Graphics/Window.hpp"
#include "Navigation.hpp"

Orbit::Orbit(PlanetInfo const& planet, float const radius, sf::Color const& color)
    : m_state{true}, m_radius{radius}, m_owner{planet}
{
    sf::Color const outline_color { color.r, color.g, color.b, visual_ring_outline_alpha };
    sf::Color const fill_color { color.r, color.g, color.b, visual_ring_fill_alpha };

    int n { 1 };
    for (auto& ring : m_rings)
    {
        // Use a power function to bunch rings closer to the planet
        float const ratio { static_cast<float>(n) / static_cast<float>(visual_ring_count) };
        float const ring_space = m_radius - m_owner.radius;
        float current_radius{ m_owner.radius + ring_space * std::pow(ratio, 1.0f + visual_ring_spacing_factor) };

        // force the last orbit a little bit further out
        // NOTE: WHY?
        // because it will help the player aim;
        // if they aim for orbit radius,
        // the gravity of the planet will
        // pull them in as they travel towards the planet;
        // fucking up their pathing.
        // but pushing this ring outwards will make
        // the player aim a little extra outwards to compensate.
        if (n == visual_ring_count) current_radius += visual_outer_ring_offset;

        ring.setRadius(current_radius);
        ring.setOrigin({current_radius, current_radius});
        ring.setPosition(m_owner.position);
        ring.setFillColor(fill_color);
        ring.setOutlineColor(outline_color);
        ring.setOutlineThickness(2.0f);

        ++n;
    }
}

sf::Vector2f Orbit::calculate_force(Player const& player) const
{
    if (!m_state) return {0.0f, 0.0f};

    sf::Vector2f const distance_vec = player.get_distance_vec(m_owner);
    float const distance = distance_vec.length();

    if (distance <= 1.0f) return {0.0f, 0.0f}; // Too close = massive force

    sf::Vector2f const direction { -distance_vec.normalized() };
    float const force_magnitude { (Navigation::G * m_owner.mass) / (distance * distance) };

    return direction * force_magnitude;
}

void Orbit::apply_force(Player& player) const
{
    player.accelerate(calculate_force(player));
}

void Orbit::draw() const
{
    if (!m_state) return;
    for (auto const& ring : m_rings)
        Window.draw(ring);
}