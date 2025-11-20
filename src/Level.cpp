#include "Level.hpp"
#include "Navigation.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"

void Level::generate(Player const& player)
{
    /* Planet Generation */
    m_planets.reserve(planet_count);

    float const v_target_sq = player.get_velocity().lengthSquared();

    std::uniform_real_distribution x_dist {x_distribution.x, x_distribution.y};
    std::uniform_real_distribution y_dist {y_distribution.x, y_distribution.y};
    std::uniform_real_distribution dmt_dist{planet_determinant_distribution.x, planet_determinant_distribution.y};

    while (m_planets.size() < planet_count)
    {
        sf::Vector2f const position {x_dist(m_rng), y_dist(m_rng)};

        float const dmt = dmt_dist(m_rng);
        float const radius = World.scale_distance(dmt * planet_radius_scaling_factor);
        float const orbit_radius = World.scale_distance(dmt * orbit_radius_scaling_factor);

        /* Check for overlap with existing planets */
        bool overlap = false;
        for (Planet const& other : m_planets)
        {
            float const min_distance =
                planet_padding + orbit_radius
                + other.get_orbit().get_radius();

            auto delta = position - other.get_position();
            overlap = delta.length() < min_distance;
            if (overlap) break;
        }

        if (overlap) continue;
        m_planets.emplace_back(
            position,
            (v_target_sq * orbit_radius) / Navigation::G,
            radius,
            orbit_radius
        );
    }
}
