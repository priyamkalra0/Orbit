#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "Entity/PlanetInfo.hpp"
#include "Graphics/World.hpp"

using ThrusterArray = std::array<sf::ConvexShape, 4>;

enum class PlayerState
{
    FarOutsideOrbit,
    NearOutsideOrbit,
    SomewhereOutsideOrbit,

    SomewhereInsideOrbit,
    InsideSmoothingRing,
    InTargetOrbit,
    InStableOrbit, /* Not target; but stable "enough" */

    Exploding,
};

class Player
{
public:
    explicit Player();

    /* Configuration Parameters */
    constexpr static float param_target_orbital_velocity { World::scale_distance(500.0f) };
    constexpr static float param_max_drift_velocity { World::scale_distance(1000.0f) };
    constexpr static float param_orbit_error_tolerance { 15.0f }; // +- error within target radius = "in orbit"
    constexpr static float param_orbit_far_distance_factor { 1.5f }; // governs how far away is considered "far outside orbit"

    /* Visual Configuration Parameters */
    constexpr static float param_visual_core_radius { 20.0f };
    constexpr static sf::Color param_visual_core_color { Color::hwb(0, 10, 90) };
    constexpr static sf::Color param_visual_thruster_color { Color::hwb(0, 70, 30) };
    constexpr static sf::Vector2f param_visual_thruster_size { 30.0f, 30.0f }; // {width, height}
    constexpr static float param_visual_thruster_base_half_width { param_visual_thruster_size.x / 2.0f };
    constexpr static float param_visual_thruster_height { param_visual_thruster_size.y };
    constexpr static float param_visual_thruster_offset { -10.0f };
    constexpr static uint32_t param_visual_explosion_particle_count { 100 };

    void update();
    void draw() const;

    void accelerate(sf::Vector2f const& force);
    void explode();

    void reset();
    bool is(PlayerState const& state) const;

    sf::Vector2f const& get_position() const { return m_position; }
    sf::Vector2f get_velocity() const;

    sf::Vector2f get_distance_vec(sf::Vector2f const& from_pos) const;
    float get_distance(sf::Vector2f const& from_pos) const;
    float get_distance_squared(sf::Vector2f const& from_pos) const;

    sf::Vector2f get_radial_velocity_vector() const;
    sf::Vector2f get_tangential_velocity_vector() const;

    void invert_velocity();
    void set_velocity(sf::Vector2f const& new_velocity); /* NOTE: WILL NULL THE ACCELERATION */
    void set_position(sf::Vector2f const& position); /* NOTE: WILL NULL THE VELOCITY */

    sf::Shape const& get_core() const { return m_core; }
    ThrusterArray const& get_thrusters() const { return m_thrusters; }

private:
    void init_shapes();

    bool m_exploding { false };

    sf::Vector2f m_position;
    sf::Vector2f m_previous_position;
    sf::Vector2f m_acceleration;

    sf::CircleShape m_core;
    ThrusterArray m_thrusters;
};
