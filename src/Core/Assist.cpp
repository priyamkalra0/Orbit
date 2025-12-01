#include <iostream>
#include "Core/Assist.hpp"
#include "Core/Navigation.hpp"
#include "Entity/Planet.hpp"
#include "Entity/Orbit.hpp"
#include "Graphics/Window.hpp"

Assist::Assist()
{
    std::cout
        << "Navigation::Navigation(): "
        << "[assist parameters] "
        << "\n Radial Smoothing Threshold: "
        << param_assist_radial_smoothing_threshold
        << "\n Radial Smoothing Power: "
        << param_assist_radial_smoothing_power
        << "\n Tangential Correction Power: "
        << param_assist_tangent_boosting_power
        << "\n Tangential Target Velocity: "
        << param_assist_tangent_target_velocity
        << "\n Radial Smoothing Ring Size: "
        << param_assist_radial_smoothing_ring_size
        << "\n";
}

void Assist::draw() const
{
    Window.draw(m_smoothing_ring_inner);
    Window.draw(m_smoothing_ring_outer);
    Window.draw(m_target_radius_ring);
}

void Assist::update()
{
    assert(m_player);

    auto& ctx = Navigation.get_context();

    Planet& target_planet = ctx.target_planet;
    Orbit const& target_orbit = ctx.target_orbit;

    float const target_radius { target_orbit.get_radius() };
    auto const orbit_origin { target_orbit.get_origin() };

    init_target_radius_ring(
        orbit_origin,
        target_radius
    );

    init_smoothing_ring_shape(orbit_origin);

    auto v_radial = ctx.player_radial_v;
    auto v_tangent = ctx.player_tangent_v;

    std::cout
        << "[core/navigation] [current state] "
        << "v_radial: " << v_radial.length()
        << ", v_tangential: " << v_tangent.length()
        << ", error: " << ctx.player_error
        << "\n";


    /* Planet Mass Boosting */
    if (m_player->is(PlayerState::SomewhereOutsideOrbit))
    {
        std::cout
            << "[core/navigation] "
            << "[planet mass boosted ("
            << param_assist_planet_mass_boosting_power << ")] "
            << target_planet.get_mass() << " -> ";

        target_planet.set_mass(
            (1.0f + param_assist_planet_mass_boosting_power) *
            (v_tangent.lengthSquared() * target_radius) / Navigation::G
        );

        std::cout << target_planet.get_mass() << "\n";
    }

    /* Everything beyond this is only applied
     * if the player is in the smoothing ring
     * and the planet's orbit is turned on */
    if (!m_player->is(PlayerState::InsideSmoothingRing)) return;

    if (v_radial.length() > param_assist_radial_smoothing_threshold)
    {
        std::cout
            << "[core/navigation] "
            << "[frame skipped!] incident radial velocity too high."
            << "\n";

        return; // Other assistance is not provided if radial smoothing did not happen
    }

    // Show that the smoothing ring is active
    m_smoothing_ring_inner.setOutlineColor(sf::Color::Yellow);
    m_smoothing_ring_outer.setOutlineColor(sf::Color::Yellow);

    /* Radial Smoothing */
    std::cout
        << "[core/navigation] "
        << "[radial smoothing success ("
        << -param_assist_radial_smoothing_power << ")] "
        << v_radial.length() << " -> ";

    v_radial *= (1.0f - param_assist_radial_smoothing_power);

    std::cout << v_radial.length() << "\n";

    /* Addon: Tangential Correction */
    float const v_tangent_error = \
        param_assist_tangent_target_velocity - v_tangent.length();

    if (std::abs(v_tangent_error) > param_assist_tangent_correction_tolerance_factor)
    {
        float const tangent_correction_power {
            (v_tangent_error > 0.0f)
            ? param_assist_tangent_boosting_power
            : -param_assist_tangent_smoothing_power
        };

        std::cout
            << "[core/navigation] "
            << "[tangential correction applied ("
            << tangent_correction_power << ")] "
            << v_tangent.length() << " -> ";

        v_tangent *= (1.0f + tangent_correction_power);

        std::cout << v_tangent.length() << std::endl;
    }

    /* Addon: Planet Mass Adjustment */
    std::cout
        << "[core/navigation] "
        << "[planet mass adjusted] "
        << target_planet.get_mass() << " -> ";

    target_planet.set_mass(
        (v_tangent.lengthSquared() * (target_radius + ctx.player_error))
        / Navigation::G
    );

    std::cout << target_planet.get_mass() << "\n";

    m_player->set_velocity(v_radial + v_tangent);
}

void Assist::init_target_radius_ring(sf::Vector2f const& position, float const radius)
{
    m_target_radius_ring.setRadius(radius);
    m_target_radius_ring.setOrigin({radius, radius});
    m_target_radius_ring.setFillColor(sf::Color::Transparent);
    m_target_radius_ring.setOutlineColor(sf::Color::Red);
    m_target_radius_ring.setOutlineThickness(2.0f);
    m_target_radius_ring.setPosition(position);
}

void Assist::init_smoothing_ring_shape(sf::Vector2f const& position)
{    auto const& [inner_size, outer_size] = \
        param_assist_radial_smoothing_ring_region_size;

    float const inner_radius = m_target_radius_ring.getRadius() - inner_size;
    float const outer_radius = m_target_radius_ring.getRadius() + outer_size;

    m_smoothing_ring_inner.setRadius(inner_radius);
    m_smoothing_ring_inner.setOrigin({inner_radius, inner_radius});
    m_smoothing_ring_inner.setFillColor(sf::Color::Transparent);
    m_smoothing_ring_inner.setOutlineColor(sf::Color::Green);
    m_smoothing_ring_inner.setOutlineThickness(2.0f);
    m_smoothing_ring_inner.setPosition(position);

    m_smoothing_ring_outer.setRadius(outer_radius);
    m_smoothing_ring_outer.setOrigin({outer_radius, outer_radius});
    m_smoothing_ring_outer.setFillColor(sf::Color::Transparent);
    m_smoothing_ring_outer.setOutlineColor(sf::Color::Green);
    m_smoothing_ring_outer.setOutlineThickness(2.0f);
    m_smoothing_ring_outer.setPosition(position);
}
