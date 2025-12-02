#pragma once

#include "Math/Vector2.hpp"

class World
{
public:
    World() = delete;

    constexpr static sf::Vector2u param_design_resolution {1920, 1080};
    constexpr static sf::Vector2u param_internal_resolution {2560, 1440};

    [[nodiscard]] constexpr static float scale_x(float const x)
    {
        return x * m_scale_factor_x;
    }

    [[nodiscard]] constexpr static float scale_y(float const y)
    {
        return y * m_scale_factor_y;
    }

    [[nodiscard]] constexpr static sf::Vector2f scale_position(sf::Vector2f const& static_position)
    {
        return {static_position.x * m_scale_factor_x, static_position.y * m_scale_factor_y};
    }

    [[nodiscard]] constexpr static float scale_distance(float const static_distance)
    {
        // For distances, we can choose to scale by x, y, or an average.
        // For now, let's scale uniformly by the y-factor, as height is often a good reference for 2D games.
        return static_distance * m_scale_factor_y;
    }

    [[nodiscard]] constexpr static sf::Vector2f scale_velocity(sf::Vector2f const& static_velocity)
    {
        // Velocity scales linearly with distance over time.
        // So, if distances scale by factor X and Y, velocities also scale by factor X and Y.
        return {static_velocity.x * m_scale_factor_x, static_velocity.y * m_scale_factor_y};
    }

private:
    constexpr static float m_scale_factor_x { static_cast<float>(param_internal_resolution.x) / static_cast<float>(param_design_resolution.x) };
    constexpr static float m_scale_factor_y { static_cast<float>(param_internal_resolution.y) / static_cast<float>(param_design_resolution.y) };
};