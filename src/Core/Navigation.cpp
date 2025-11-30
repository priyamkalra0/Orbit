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

    return {
    .player_error = player_error,
    .target_planet = ref_target,
    .target_orbit = ref_target.get_orbit(),
    .nearest_planet = ref_nearest,
    .nearest_orbit = ref_nearest.get_orbit(),
    .previous_planet = (has_context()) ? m_context->target_planet : ref_target,
    .previous_orbit = (has_context()) ? m_context->target_orbit : ref_target.get_orbit(),
    };
}

void Navigation::draw() const { /* Nothing as of now */ }

void Navigation::update() { load_context(); /* Reload context once every frame */ }