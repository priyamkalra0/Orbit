#include "Core/Navigation.hpp"
#include "Core/Level.hpp"


template <typename T>
using NullableRef = std::optional<std::reference_wrapper<T>>;

/* Since I want the navigation context
 * to provide guaranteed references; this method
 * takes on the messy job of finding planets
 * (nullable references -> clean references)
 * The ASSERT should make sure
 * null references are never created.
 * Anyway, this method should only be called *after*
 * the player is created and the level is generated.
 */
NavigationContext Navigation::make_context() const
{
    assert (m_player);

    float player_error { -1 };
    float min_distance_orbit_unchecked { -1 };
    float min_distance_orbit_checked { -1 };


    NullableRef<Planet> candidate_nearest; /* Candidate for nearest planet */
    NullableRef<Planet> candidate_target; /* Candidate for target planet */

    for (Planet& planet : Level.get_planets())
    {
        float const current_distance { m_player->get_distance(planet.get_info()) };
        if (min_distance_orbit_unchecked < 0.0f || current_distance < min_distance_orbit_unchecked)
        {
            min_distance_orbit_unchecked = current_distance;
            candidate_nearest.emplace(planet);
        }

        if (auto const& orbit = planet.get_orbit(); orbit.is_on()) {
            if (min_distance_orbit_checked > 0.0f && current_distance > min_distance_orbit_checked) continue;
            min_distance_orbit_checked = current_distance;
            player_error = current_distance - orbit.get_radius();
            candidate_target.emplace(planet);
        }
    }

    assert (
        candidate_target.has_value()
        && candidate_nearest.has_value()
    );

    /* Now we can safely convert to concrete references */
    Planet& ref_nearest = candidate_nearest->get();
    Planet& ref_target = candidate_target->get();

    /* We still need to do some work to find the previous planet */
    Planet& ref_prev = ctx_get_previous_planet(ref_target);

    /* Player's relative velocity components */
    sf::Vector2f v_radial { m_player->get_radial_velocity_vector(ref_target.get_info()) };
    if (v_radial.length() < 1.0f) v_radial *= 0.0f; // We don't care about values that small

    sf::Vector2f v_tangent { m_player->get_tangential_velocity_vector(ref_target.get_info()) };
    if (v_tangent.length() < 1.0f) v_tangent *= 0.0f;

    return {
    .player_radial_v = v_radial,
    .player_tangent_v = v_tangent,
    .player_error = player_error,
    .target_planet = ref_target,
    .target_orbit = ref_target.get_orbit(),
    .nearest_planet = ref_nearest,
    .nearest_orbit = ref_nearest.get_orbit(),
    .previous_planet = ref_prev,
    .previous_orbit = ref_prev.get_orbit(),
    };
}

Planet& Navigation::ctx_get_previous_planet(Planet& current_target_ref) const
{
    /* Context is NULL => game just started => previous planet = current planet */
    if (!has_context()) return current_target_ref;

    if (
        &m_context->target_planet != &current_target_ref /* Target planet has changed */
        && m_player->is(PlayerState::InTargetOrbit) /* Player was stable in orbit */
    )  return m_context->target_planet; /* Update previous planet; allowing player to respawn there */

    return m_context->previous_planet; /* Otherwise previous planet remains unchanged; player will respawn further back */
}

void Navigation::draw() const { /* Nothing as of now */ }

void Navigation::update() { load_context(); /* Reload context once every frame */ }