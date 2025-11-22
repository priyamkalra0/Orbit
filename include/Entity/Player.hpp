#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/World.hpp"
#include "PlanetInfo.hpp"

enum class PlayerState
{
    FarOutsideOrbit,
    NearOutsideOrbit,
    SomewhereOutsideOrbit,

    SomewhereInsideNullOrbit,
    SomewhereInsideOrbit,
    InsideSmoothingRing,
    ExactlyInTargetOrbit,
};

class Player
{
public:
    explicit Player(sf::Vector2f const& position = {0, 0}, sf::Vector2f const& initial_velocity = {0, 0});

    /* Configuration Parameters */
    float const param_max_velocity { World.scale_distance(1000.0f) };

    void update();
    void draw() const;

    void accelerate(sf::Vector2f const& force);
    bool is(PlayerState const& state) const;

    sf::Vector2f const& get_position() const { return m_position; }
    sf::Vector2f get_velocity() const;

    sf::Vector2f get_distance_vec(PlanetInfo const& planet) const;
    float get_distance(PlanetInfo const& planet) const;
    float get_distance_squared(PlanetInfo const& planet) const;

    sf::Vector2f get_radial_velocity_vector(PlanetInfo const& planet) const;
    sf::Vector2f get_tangential_velocity_vector(PlanetInfo const& planet) const;

    void set_velocity(sf::Vector2f const& new_velocity); /* NOTE: WILL NULL THE ACCELERATION */
    void set_position(sf::Vector2f const& position); /* NOTE: WILL NULL THE VELOCITY */

public:
    /* Quick and very bad way to inject navigation context into player */
    float _ctx_Navigation_SignedError { 0.0f };
    float _ctx_Navigation_SmoothingRingSizeInner { 0.0f };
    float _ctx_Navigation_SmoothingRingSizeOuter { 0.0f };

private:
    sf::Vector2f m_position;
    sf::Vector2f m_previous_position;
    sf::Vector2f m_acceleration;

    sf::ConvexShape m_shape;
};
