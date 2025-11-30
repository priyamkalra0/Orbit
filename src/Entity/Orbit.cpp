#include "Entity/Orbit.hpp"
#include "Core/Navigation.hpp"
#include "Entity/PlanetInfo.hpp"
#include "Entity/Player.hpp"
#include "Graphics/Window.hpp"
#include "Math/Vector2.hpp"

Orbit::Orbit(PlanetInfo const& planet, float const radius, sf::Color const& color)
    : m_state{true}, m_radius{radius}, m_color{color}, m_owner{planet} { init_rings(); }

void Orbit::init_rings(float const highlight_factor)
{
    sf::Color const outline_color { m_color.r, m_color.g, m_color.b, param_visual_ring_outline_alpha };

    int n { 1 };
    for (auto& ring : m_rings)
    {
        bool const is_inner_ring = (n < param_visual_ring_count);

        float const fill_alpha_coefficient =
            (m_state)
            ? (1.0f + highlight_factor * is_inner_ring) // on = inner rings highlighted
            : 0.4f; // off = 40% alpha

        sf::Color const fill_color {
            m_color.r, m_color.g, m_color.b,
            static_cast<std::uint8_t>(param_visual_ring_fill_alpha * fill_alpha_coefficient) // light up effect
        };

        // Use a power function to bunch rings closer to the planet
        float const ratio { static_cast<float>(n) / static_cast<float>(param_visual_ring_count) };
        float const ring_space { m_radius - m_owner.radius };
        float current_radius{ m_owner.radius + ring_space * std::pow(ratio, 1.0f + param_visual_ring_spacing_factor) };

        // force the last orbit a little bit further out
        // NOTE: WHY?
        // because it will help the player aim;
        // if they aim for orbit radius,
        // the gravity of the planet will
        // pull them in as they travel towards the planet;
        // fucking up their pathing.
        // but pushing this ring outwards will make
        // the player aim a little extra outwards to compensate.
        if (!is_inner_ring) current_radius += param_visual_outer_ring_offset;

        ring.setRadius(current_radius);
        ring.setOrigin({current_radius, current_radius});
        ring.setPosition(get_origin());
        ring.setFillColor(fill_color);
        ring.setOutlineColor(outline_color);
        ring.setOutlineThickness(2.0f);

        ++n;
    }
}

sf::Vector2f Orbit::calculate_force(Player const& player)
{
    if (!m_state) return {0.0f, 0.0f};

    sf::Vector2f const distance_vec { player.get_distance_vec(m_owner) };
    float const distance { distance_vec.length() };

    if (distance <= 1.0f) return {0.0f, 0.0f}; // Too close = massive force

    /* Highlight active orbit */
    float const highlight_distance_factor = std::min(
        param_visual_ring_highlight_clamp,
        1.0f - std::min(1.0f, distance / (param_visual_ring_highlight_factor * m_radius))
        );
    init_rings(1.5f * param_visual_ring_highlight_factor * highlight_distance_factor);

    sf::Vector2f const direction { -distance_vec.normalized() };
    float const force_magnitude { (Navigation::G * m_owner.mass) / (distance * distance) };

    return direction * force_magnitude;
}

void Orbit::apply_force(Player& player)
{
    player.accelerate(calculate_force(player));
}

void Orbit::draw() const
{
    for (auto const& ring : m_rings)
        Window.draw(ring);
}