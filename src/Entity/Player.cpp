#include <iostream>

#include "Entity/Player.hpp"
#include "Core/Assist.hpp"
#include "Core/Collision.hpp"
#include "Core/Navigation.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Particles.hpp"
#include "Math/Vector2.hpp"

Player::Player()
{
    // Verlet integration needs the previous position
    m_previous_position = m_position; // aka NULL velocity

    m_shape.setPointCount(3);
    m_shape.setPoint(0, {0.0f, -20.0f});
    m_shape.setPoint(1, {10.0f, 10.0f});
    m_shape.setPoint(2, {-10.0f, 10.0f});
    m_shape.setOrigin({0.0f, 0.0f});
    m_shape.setPosition(m_position);
    m_shape.setFillColor(sf::Color::Red);
}

void Player::draw() const
{
    if (m_exploding) return;
    Window.draw(m_shape);
}

void Player::accelerate(sf::Vector2f const& force)
{
    m_acceleration += force; // a = f/m = f (player has unit mass)
}

void Player::explode()
{
    m_exploding = true;
    ParticleEmitter.emit(
        param_visual_explosion_particle_count,
        m_position
    );
}

void Player::reset()
{
    Random respawn_rand; // FIXME: Probably should not make a new one every time
    auto& ctx { Navigation.get_context() };

    /* Respawn when
     * pressing R: at target planet (debug mode only)
     * exploding: at previous planet */
    Orbit& respawn_orbit {
        is(PlayerState::Exploding)
        ? ctx.previous_orbit
        : ctx.target_orbit
    };
    respawn_orbit.turn_on(); // ensure it's on

    const size_t offset_axis { respawn_rand.get<bool>() }; // 0 or 1

    float vec_buf[2] { 0 };
    vec_buf[offset_axis] = \
        respawn_rand.sign<float>()
        * respawn_orbit.get_radius();

    set_position(
        respawn_orbit.get_origin()
        + sf::Vector2f{vec_buf[0], vec_buf[1]}
    ); // at target orbit

    vec_buf[!offset_axis] = // tangent axis is opposite
        respawn_rand.sign<float>()
        * param_target_orbital_velocity;
    vec_buf[offset_axis] = vec_buf[!offset_axis] * 0.1f; // some radial noise
    set_velocity({
        vec_buf[0],
        vec_buf[1]
    }); // target velocity, nearly tangent direction

    if (m_exploding) m_exploding = false; // Start drawing player again.
}

bool Player::is(PlayerState const& state) const
{
    auto& ctx { Navigation.get_context() };

    auto const& [smoothing_ring_inner_size, smoothing_ring_outer_size] {
        Assist::param_assist_radial_smoothing_ring_region_size
    };

    switch (state)
    {
    case PlayerState::FarOutsideOrbit: return (ctx.player_error > param_orbit_far_distance_factor * smoothing_ring_outer_size);
    case PlayerState::SomewhereOutsideOrbit: return !is(PlayerState::SomewhereInsideOrbit);
    // case PlayerState::NotInTargetOrbit: return (ctx.player_error > param_orbit_error_tolerance);
    case PlayerState::NearOutsideOrbit: return is(PlayerState::SomewhereOutsideOrbit)
                                             & !is(PlayerState::FarOutsideOrbit);

    case PlayerState::InTargetOrbit: return (std::abs(ctx.player_error) <= param_orbit_error_tolerance);
    case PlayerState::InsideSmoothingRing: return (
        ctx.player_error > -smoothing_ring_inner_size
        && is(PlayerState::SomewhereInsideOrbit) //ctx.player_error < smoothing_ring_outer_size
    );
    case PlayerState::SomewhereInsideOrbit: return (ctx.player_error < smoothing_ring_outer_size);
    case PlayerState::InStableOrbit: return is(PlayerState::SomewhereInsideOrbit)
                                         && ctx.player_radial_v.length()
                                            < Assist::param_assist_radial_smoothing_threshold;

    case PlayerState::Exploding: return m_exploding;

    default: return false;
    }
}

void Player::update()
{
    std::cout << "[entity/player] [current state] is(InTargetOrbit): "
        << is(PlayerState::InTargetOrbit) << "\n";
    std::cout << "[entity/player] [current state] is(InsideSmoothingRing): "
        << is(PlayerState::InsideSmoothingRing) << "\n";
    std::cout << "[entity/player] [current state] is(SomewhereOutsideOrbit): "
        << is(PlayerState::SomewhereOutsideOrbit) << "\n";
    std::cout << "[entity/player] [current state] is(FarOutsideOrbit): "
        << is(PlayerState::FarOutsideOrbit) << "\n";
    std::cout << "[entity/player] [current state] is(NearOutsideOrbit): "
        << is(PlayerState::NearOutsideOrbit) << "\n";

    if (m_position == m_previous_position)
        return reset(); /* Game start; bind to planet nearest to origin */

    if (!m_exploding && Collision::with_any_planet(m_shape))
        return explode(); /* Particle emitter takes over, blocking main Game::update() loop */

    /* Particle emitter stopped blocking; reset player and continue gameplay */
    if (is(PlayerState::Exploding))
        reset(); // Not returning; since the player shape needs to be synced after reset

    float const dt { Window.get_delta_time() };

    // Clamp velocity
    sf::Vector2f const current_velocity { get_velocity() };
    float const current_velocity_mag { current_velocity.length() };

    if (
        is(PlayerState::FarOutsideOrbit) /* Smoothing ring manages velocity inside orbit */
        && current_velocity_mag > param_max_drift_velocity
    )
    {
        std::cout << "[entity/player] [warning] drift velocity clamped: ";
        std::cout << current_velocity_mag << " -> ";

        set_velocity(current_velocity.normalized() * param_max_drift_velocity);

        std::cout << get_velocity().length();

    }

    // Verlet integration
    sf::Vector2f const current_position { m_position };
    m_position = 2.0f * current_position - m_previous_position + m_acceleration * (dt * dt);
    m_previous_position = current_position;
    m_acceleration = {0.0f, 0.0f};

    // Sync the drawable shape
    m_shape.setPosition(m_position);

    // Calculate rotation based on velocity
    if (current_velocity_mag < 0.0f) return;
    float const angle_radians { std::atan2(current_velocity.y, current_velocity.x) };
    float const angle_degrees { sf::radians(angle_radians).asDegrees() };
    m_shape.setRotation(sf::degrees(angle_degrees + 90.0f));
}

sf::Vector2f Player::get_velocity() const
{
    float const dt { Window.get_delta_time() };
    return (m_position - m_previous_position) / dt;
}

void Player::invert_velocity()
{
    std::swap(m_position, m_previous_position);
}

sf::Vector2f Player::get_distance_vec(sf::Vector2f const& from_pos) const
{
    return get_position() - from_pos;
}

float Player::get_distance(sf::Vector2f const& from_pos) const
{
    return get_distance_vec(from_pos).length();
}

float Player::get_distance_squared(sf::Vector2f const& from_pos) const
{
    return get_distance_vec(from_pos).lengthSquared();
}

void Player::set_velocity(sf::Vector2f const& new_velocity)
{
    // Since we are using Verlet Integration internally,
    // all we need to do is overwrite m_previous_position
    float const dt { Window.get_delta_time() };
    m_previous_position = m_position - new_velocity * dt;
    m_acceleration = {0.0f, 0.0f};
}

sf::Vector2f Player::get_radial_velocity_vector() const
{
    auto const& ctx { Navigation.get_context() };
    auto const& origin { ctx.target_orbit.get_origin() };
    sf::Vector2f const radial_direction { get_distance_vec(origin).normalized() };
    return get_velocity().projectedOnto(radial_direction);
}

sf::Vector2f Player::get_tangential_velocity_vector() const
{
    return get_velocity() - get_radial_velocity_vector();
}

void Player::set_position(sf::Vector2f const& position)
{
    m_position = position;
    m_previous_position = position;
}
