#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "PlanetInfo.hpp"
#include "Graphics/World.hpp"
#include "Player.hpp"

class Orbit
{
public:
    Orbit(PlanetInfo const& planet, float radius, sf::Color const& color = sf::Color{150, 150, 150});

    /* Visual Configuration Parameters */
    constexpr static float visual_ring_spacing_factor { 0.50f };
    float const visual_outer_ring_offset { World.scale_distance(35.0f) };

    void draw() const;

    [[nodiscard]] sf::Vector2f calculate_force(Player const& player) const;
    void apply_force(Player& player) const;

    [[nodiscard]] float get_radius() const { return m_radius; };

    void turn_on() { m_state = true;}
    void turn_off() { m_state = false;}
    void toggle() { m_state = !m_state;}

    [[nodiscard]] bool is_on() const { return m_state; }

private:
    bool m_state;
    float m_radius;
    PlanetInfo const& m_owner;
    std::vector<sf::CircleShape> m_rings;
};
