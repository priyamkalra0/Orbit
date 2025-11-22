#include "Level.hpp"
#include "Navigation.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"
#include "Graphics/Color.hpp"

void Level::generate(Player const& player)
{
    /* Planet Generation */
    m_planets.reserve(planet_count);

    float const v_target_sq { player.get_velocity().lengthSquared() };

    while (m_planets.size() < planet_count)
    {
        auto const position {
            m_random.get<sf::Vector2f>(
            planet_position_distribution[0],
            planet_position_distribution[1]
        )};

        float const dmt {
            m_random.get(
            planet_determinant_distribution[0],
            planet_determinant_distribution[1]
        )};
        float const radius { World.scale_distance(dmt * planet_radius_scaling_factor) };
        float const orbit_radius { World.scale_distance(dmt * orbit_radius_scaling_factor) };

        /* Check for overlap with existing planets */
        bool overlap { false };
        for (Planet const& other : m_planets)
        {
            float const min_distance {
                planet_padding + orbit_radius
                + other.get_orbit().get_radius()
            };

            auto delta { position - other.get_position() };
            overlap = delta.length() < min_distance;
            if (overlap) break;
        }

        if (overlap) continue;

        auto const orbit_color {
            Color::hwb(
                m_random.get<float>(0, 359),
                visual_orbit_color_whiteness,
                visual_orbit_color_blackness
            )
        };

        m_planets.emplace_back(
            position,
            (v_target_sq * orbit_radius) / Navigation::G,
            radius,
            visual_planet_color,
            orbit_radius,
            orbit_color
        );
    }
}
