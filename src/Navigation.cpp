#include <iostream>

#include "Graphics/Window.hpp"
#include "Navigation.hpp"
#include "Level.hpp"

Navigation::Navigation(Player& player)
    : m_player{player}
{
    std::cout
        << "Navigation::Navigation(): "
        << "[assist parameters] "
        << "\n Radial Smoothing Threshold: "
        << radial_smoothing_threshold
        << "\n Radial Smoothing Power: "
        << radial_smoothing_power
        << "\n Tangential Correction Power: "
        << tangential_boosting_power
        << "\n Tangential Target Velocity: "
        << tangential_target_velocity
        << "\n Radial Smoothing Ring Size: "
        << radial_smoothing_ring_size
        << "\n";

}

Planet& Navigation::get_active_planet() const
{
    Planet* active_planet {nullptr};

    float smallest_distance {-1};
    for (auto& planet : Level.get_planets())
    {
        if (!planet.get_orbit().is_on()) continue;
        float const d = m_player.get_distance_squared(planet.get_info());
        if (smallest_distance > 0.0f && d > smallest_distance) continue;
        smallest_distance = d;
        active_planet = &planet;
    }

    return *active_planet;
}

void Navigation::draw() const
{
    Window.draw(m_smoothing_ring_inner);
    Window.draw(m_smoothing_ring_outer);
}

void Navigation::apply_assistance()
{
    Planet& planet = get_active_planet();
    Orbit const& orbit = planet.get_orbit();

    float const target_radius = orbit.get_radius();

    init_smoothing_ring_shape(
        planet.get_position(),
        target_radius - radial_smoothing_ring_size / 2.0f,
        target_radius + radial_smoothing_ring_size / 2.0f
    );

    float const distance = m_player.get_distance(planet.get_info());
    float const signed_error = distance - target_radius;

    sf::Vector2f v_radial = m_player.get_radial_velocity_vector(planet.get_info());
    if (v_radial.length() < 1.0f) v_radial *= 0.0f; // We don't care about values that small

    sf::Vector2f v_tangential = m_player.get_tangential_velocity_vector(planet.get_info());
    if (v_tangential.length() < 1.0f) v_tangential *= 0.0f;

    std::cout
        << "Navigation::apply_assistance(): [current state] "
        << "v_radial: " << v_radial.length()
        << ", v_tangential: " << v_tangential.length()
        << ", error: " << signed_error
        << "\n";


    /* Planet Mass Boosting */
    if (signed_error > 0.0f)
    {
        std::cout
            << "Navigation::apply_assistance(): "
            << "[planet mass boosted ("
            << planet_mass_boosting_power << ")] "
            << planet.get_mass() << " -> ";

        planet.set_mass(
            (1.0f + planet_mass_boosting_power) *
            (v_tangential.lengthSquared() * target_radius) / G
        );

        std::cout << planet.get_mass() << "\n";

    }

    /* Everything beyond this is only applied
     * if the player is in the smoothing ring
     * and the planet's orbit is turned on */
    if (
        !orbit.is_on()
        || std::abs(signed_error) > (radial_smoothing_ring_size / 2.0f)
    ) return;


    // Show that the smoothing ring is active
    m_smoothing_ring_inner.setOutlineColor(sf::Color::Yellow);
    m_smoothing_ring_outer.setOutlineColor(sf::Color::Yellow);

    if (v_radial.length() > radial_smoothing_threshold)
    {
        std::cout
            << "Navigation::apply_assistance(): "
            << "[frame skipped!] incident radial velocity too high."
            << "\n";

        return; // Other assistance is not provided if radial smoothing did not happen
    }

    /* Radial Smoothing */
    std::cout
        << "Navigation::apply_assistance(): "
        << "[radial smoothing success ("
        << -radial_smoothing_power << ")] "
        << v_radial.length() << " -> ";

    v_radial *= (1.0f - radial_smoothing_power);

    std::cout << v_radial.length() << "\n";


    /* Addon: Planet Mass Adjustment */
    std::cout
        << "Navigation::apply_assistance(): "
        << "[planet mass adjusted] "
        << planet.get_mass() << " -> ";

    planet.set_mass((v_tangential.lengthSquared() * distance) / G);

    std::cout << planet.get_mass() << "\n";

    /* Addon: Tangential Correction */
    float const correction_power =
        (v_tangential.length() < tangential_target_velocity)
        ? tangential_boosting_power
        : -tangential_smoothing_power;

    std::cout
        << "Navigation::apply_assistance(): "
        << "[tangential correction applied ("
        << correction_power << ")] "
        << v_tangential.length() << " -> ";

    v_tangential *= (1.0f + correction_power);

    std::cout << v_tangential.length() << std::endl;

    m_player.set_velocity(v_radial + v_tangential);
}

void Navigation::init_smoothing_ring_shape(sf::Vector2f const& position, float const inner_radius, float const outer_radius)
{
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
