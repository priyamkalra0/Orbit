#pragma once

#include <SFML/Graphics.hpp>
#include "Entity/PlanetInfo.hpp"
#include "Graphics/World.hpp"

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
    explicit Player(sf::Vector2f const& position = {0, 0}, sf::Vector2f const& initial_velocity = {0, 0});

    /* Configuration Parameters */
    constexpr static float param_target_orbital_velocity { World::scale_distance(500.0f) };
    constexpr static float param_max_drift_velocity { World::scale_distance(1000.0f) };
    constexpr static float param_orbit_error_tolerance { 15.0f }; // +- error within target radius = "in orbit"
    constexpr static float param_orbit_far_distance_factor { 1.5f }; // governs how far away is considered "far outside orbit"

    /* Visual Configuration Parameters */
    constexpr static uint32_t param_visual_explosion_particle_count { 100 };

    void update();
    void draw() const;

    void accelerate(sf::Vector2f const& force);
    void explode();

    void reset();
    bool is(PlayerState const& state) const;

    sf::Vector2f const& get_position() const { return m_position; }
    sf::Vector2f get_velocity() const;

    sf::Vector2f get_distance_vec(PlanetInfo const& planet) const;
    float get_distance(PlanetInfo const& planet) const;
    float get_distance_squared(PlanetInfo const& planet) const;

    sf::Vector2f get_radial_velocity_vector(PlanetInfo const& planet) const;
    sf::Vector2f get_tangential_velocity_vector(PlanetInfo const& planet) const;

    void invert_velocity();
    void set_velocity(sf::Vector2f const& new_velocity); /* NOTE: WILL NULL THE ACCELERATION */
    void set_position(sf::Vector2f const& position); /* NOTE: WILL NULL THE VELOCITY */

    sf::Shape const& get_shape() const { return m_shape; }

private:
    bool m_exploding { false };

    sf::Vector2f m_position;
    sf::Vector2f m_previous_position;
    sf::Vector2f m_acceleration;

    sf::ConvexShape m_shape;
};
