#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "PlanetInfo.hpp"
#include "Graphics/World.hpp"
#include "Player.hpp"

class Orbit
{
public:
    Orbit(PlanetInfo const& planet, float radius, sf::Color const& color);

    /* Visual Configuration Parameters */
    constexpr static uint32_t visual_ring_count { 6 };
    constexpr static uint8_t visual_ring_outline_alpha { 50 };
    constexpr static uint8_t visual_ring_fill_alpha { 25 };
    constexpr static float visual_ring_spacing_factor { 0.50f };
    constexpr static float visual_ring_highlight_clamp { 0.4f };
    constexpr static float visual_ring_highlight_factor { 2.0f };
    float const visual_outer_ring_offset { World.scale_distance(35.0f) };

    void draw() const;

    [[nodiscard]] sf::Vector2f calculate_force(Player const& player);
    void apply_force(Player& player);

    [[nodiscard]] float get_radius() const { return m_radius; }
    sf::Vector2f const& get_origin() const { return m_owner.position; }

    void turn_on() { m_state = true; init_rings(); }
    void turn_off() { m_state = false; init_rings(); }
    void toggle() { m_state = !m_state; init_rings(); }

    [[nodiscard]] bool is_on() const { return m_state; }

private:
    void init_rings(float highlight_factor = 0.0f);

    bool m_state;
    float m_radius;
    sf::Color m_color;
    PlanetInfo const& m_owner;
    sf::CircleShape m_rings[visual_ring_count];
};
