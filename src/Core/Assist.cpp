#include <iostream>
#include "Core/Assist.hpp"
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
        << param_assist_tangential_boosting_power
        << "\n Tangential Target Velocity: "
        << param_assist_tangential_target_velocity
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
    Planet& planet { Navigation.get_active_planet() };
    Orbit const& orbit { planet.get_orbit() };

    float const target_radius { orbit.get_radius() };

    init_target_radius_ring(
        planet.get_position(),
        target_radius
    );

    init_smoothing_ring_shape(planet.get_position());

    float const distance { m_player->get_distance(planet.get_info()) };
    // float const signed_error = distance - target_radius;

    sf::Vector2f v_radial { m_player->get_radial_velocity_vector(planet.get_info()) };
    if (v_radial.length() < 1.0f) v_radial *= 0.0f; // We don't care about values that small

    sf::Vector2f v_tangential { m_player->get_tangential_velocity_vector(planet.get_info()) };
    if (v_tangential.length() < 1.0f) v_tangential *= 0.0f;

    std::cout
        << "[core/navigation] [current state] "
        << "v_radial: " << v_radial.length()
        << ", v_tangential: " << v_tangential.length()
        //<< ", error: " << m_player->_ctx_Navigation_SignedError
        << "\n";


    /* Planet Mass Boosting */
    if (m_player->is(PlayerState::SomewhereOutsideOrbit))
    {
        std::cout
            << "[core/navigation] "
            << "[planet mass boosted ("
            << param_assist_planet_mass_boosting_power << ")] "
            << planet.get_mass() << " -> ";

        planet.set_mass(
            (1.0f + param_assist_planet_mass_boosting_power) *
            (v_tangential.lengthSquared() * target_radius) / Navigation.G
        );

        std::cout << planet.get_mass() << "\n";

    }

    /* Everything beyond this is only applied
     * if the player is in the smoothing ring
     * and the planet's orbit is turned on */
    if (!m_player->is(PlayerState::InsideSmoothingRing)) return;


    // Show that the smoothing ring is active
    m_smoothing_ring_inner.setOutlineColor(sf::Color::Yellow);
    m_smoothing_ring_outer.setOutlineColor(sf::Color::Yellow);

    if (v_radial.length() > param_assist_radial_smoothing_threshold)
    {
        std::cout
            << "[core/navigation] "
            << "[frame skipped!] incident radial velocity too high."
            << "\n";

        return; // Other assistance is not provided if radial smoothing did not happen
    }

    /* Radial Smoothing */
    std::cout
        << "[core/navigation] "
        << "[radial smoothing success ("
        << -param_assist_radial_smoothing_power << ")] "
        << v_radial.length() << " -> ";

    v_radial *= (1.0f - param_assist_radial_smoothing_power);

    std::cout << v_radial.length() << "\n";


    /* Addon: Planet Mass Adjustment */
    std::cout
        << "[core/navigation] "
        << "[planet mass adjusted] "
        << planet.get_mass() << " -> ";

    planet.set_mass((v_tangential.lengthSquared() * distance) / Navigation.G);

    std::cout << planet.get_mass() << "\n";

    /* Addon: Tangential Correction */
    float const correction_power {
        (v_tangential.length() < param_assist_tangential_target_velocity)
        ? param_assist_tangential_boosting_power
        : -param_assist_tangential_smoothing_power
    };

    std::cout
        << "[core/navigation] "
        << "[tangential correction applied ("
        << correction_power << ")] "
        << v_tangential.length() << " -> ";

    v_tangential *= (1.0f + correction_power);

    std::cout << v_tangential.length() << std::endl;

    m_player->set_velocity(v_radial + v_tangential);
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
{    auto const& [inner_radius, outer_radius] = \
        param_assist_radial_smoothing_ring_region_size;

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
