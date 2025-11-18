#pragma once

#include "Graphics/Vector2.hpp"

class World
{
public:
    World(sf::Vector2u const& design_resolution, sf::Vector2u const& current_internal_resolution)
        : m_design_resolution{design_resolution},
          m_current_internal_resolution{current_internal_resolution}
    {
        m_scale_factor_x = static_cast<float>(m_current_internal_resolution.x) / static_cast<float>(m_design_resolution.x);
        m_scale_factor_y = static_cast<float>(m_current_internal_resolution.y) / static_cast<float>(m_design_resolution.y);
    }

    [[nodiscard]] float scale_x(float const x) const
    {
        return x * m_scale_factor_x;
    }

    [[nodiscard]] float scale_y(float const y) const
    {
        return y * m_scale_factor_y;
    }

    [[nodiscard]] sf::Vector2f scale_position(sf::Vector2f const& static_position) const
    {
        return {static_position.x * m_scale_factor_x, static_position.y * m_scale_factor_y};
    }

    [[nodiscard]] float scale_distance(float const static_distance) const
    {
        // For distances, we can choose to scale by x, y, or an average.
        // For now, let's scale uniformly by the y-factor, as height is often a good reference for 2D games.
        return static_distance * m_scale_factor_y;
    }

    [[nodiscard]] sf::Vector2f scale_velocity(sf::Vector2f const& static_velocity) const
    {
        // Velocity scales linearly with distance over time.
        // So, if distances scale by factor X and Y, velocities also scale by factor X and Y.
        return {static_velocity.x * m_scale_factor_x, static_velocity.y * m_scale_factor_y};
    }

private:
    sf::Vector2u m_design_resolution;
    sf::Vector2u m_current_internal_resolution;
    float m_scale_factor_x;
    float m_scale_factor_y;
};

using World_t = World;
extern World_t World;