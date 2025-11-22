#pragma once

#include <vector>
#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"
#include "Graphics/Vector2.hpp"
#include "Math/Random.hpp"

class Level
{
public:
    Level() = default;

    void generate(Player const& player);

    std::vector<Planet>& get_planets() { return m_planets; }

    /* Level Generation Parameters */
    constexpr static uint32_t planet_count { 500 };
    float const planet_padding { World.scale_distance(250.0f) };

    constexpr static float position_distribution_scaling_factor { 8.0f };
    constexpr static sf::Vector2f planet_position_distribution[2] {
        sf::Vector2f{-1920.0f, -1080.f} * position_distribution_scaling_factor, // min
        sf::Vector2f{1920.f, 1080.0f} * position_distribution_scaling_factor // max
    };

    /* Planet Determinant: Serves as a single random seed to
     * generate both the planet's & it's orbit's radii.
     */
    constexpr static float planet_determinant_distribution[2] {7.0f, 14.0f};
    constexpr static float planet_radius_scaling_factor { 8.0f };
    constexpr static float orbit_radius_scaling_factor { 20.0f };


private:
    std::vector<Planet> m_planets;
    Random m_random;
};

using Level_t = Level;
extern Level_t Level;
