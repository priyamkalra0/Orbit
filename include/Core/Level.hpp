#pragma once

#include <utility>
#include <vector>
#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"
#include "Math/Vector2.hpp"
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
    std::pair<ufd, ufd> const planet_position_distribution {
        ufd { -1920.f * position_distribution_scaling_factor, +1920.f * position_distribution_scaling_factor }, // x
        ufd { -1080.f * position_distribution_scaling_factor, +1080.f * position_distribution_scaling_factor } // y
    };

    /* Planet Determinant: Serves as a single random seed to
     * generate both the planet's & it's orbit's radii.
     */
    urd<float> const planet_determinant_distribution { 7.0f, 14.0f };
    constexpr static float planet_radius_scaling_factor { 8.0f };
    constexpr static float orbit_radius_scaling_factor { 20.0f };

    /* Visual Parameters */
    constexpr static sf::Color visual_planet_color { sf::Color::White };
    urd<double> const visual_orbit_color_hue_distribution { 0.0, 359.0 };
    constexpr static double visual_orbit_color_whiteness { 65.0 };
    constexpr static double visual_orbit_color_blackness { 0.0 };


private:
    std::vector<Planet> m_planets;
    Random m_random;
};

using Level_t = Level;
extern Level_t Level;
