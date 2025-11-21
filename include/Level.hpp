#pragma once

#include <vector>
#include <random>
#include "Graphics/Vector2.hpp"
#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"

class Level
{
public:
    Level() : m_rng(std::random_device{}()) {}

    void generate(Player const& player);

    std::vector<Planet>& get_planets() { return m_planets; }

    /* Level Generation Parameters */
    constexpr static uint32_t planet_count { 500 };
    float const planet_padding { World.scale_distance(250.0f) };

    constexpr static float dist_scaling_factor = 8.0f;
    constexpr static auto x_distribution = sf::Vector2f{-1920.0f, 1920.0f} * dist_scaling_factor;
    constexpr static auto y_distribution = sf::Vector2f{-1080.0f, 1080.0f} * dist_scaling_factor;

    /* Planet Determinant: Serves as a single random seed to
     * generate both the planet's & it's orbit's radii.
     */
    constexpr static sf::Vector2f planet_determinant_distribution {7.0f, 14.0f};
    constexpr static float planet_radius_scaling_factor { 8.0f };
    constexpr static float orbit_radius_scaling_factor { 20.0f };


private:
    std::vector<Planet> m_planets;
    std::mt19937 m_rng;
};

using Level_t = Level;
extern Level_t Level;
