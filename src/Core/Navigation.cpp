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
    float min_distance { -1 };

    NullableRef<Planet> candidate_nearest; /* Candidate for nearest planet */
    NullableRef<Planet> candidate_target; /* Candidate for target planet */

    for (Planet& planet : Level.get_planets())
    {
        float const current_distance { m_player->get_distance(planet.get_info()) };
        if (min_distance > 0.0f && current_distance > min_distance) continue;

        min_distance = current_distance;
        candidate_nearest.emplace(planet);

        if (auto const& orbit = planet.get_orbit(); orbit.is_on()) {
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

    return {
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