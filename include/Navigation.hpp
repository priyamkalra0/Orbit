#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"

class Navigation
{
public:
    Navigation(std::vector<Planet>& planets, Player& player);

    /* Active Planet: Closest planet whose orbit is currently on */
    Planet& get_active_planet() const;
    void apply_assistance();

    void draw() const;

    /* Navigation Assist Parameters;
     anything related to position/velocity should
     use scaling methods from the `World` class */
    constexpr static float G { 10.0f };

    /* Planet Mass Boosting:
     * Makes the planet's pull stronger
     * while the player is outside the orbit */
    constexpr static float planet_mass_boosting_power { 0.3f };

    /* Radial Smoothing:
     * Helps player stabilize into orbit;
     * given that they did a sufficiently good job
     * at aiming for a tangent path to the orbit
     * that is; their radial velocity was
     * under the tolerable threshold */
    float const radial_smoothing_threshold { World.scale_distance(300.0f) };
    float const radial_smoothing_ring_size { World.scale_distance(76.0f) };
    constexpr static float radial_smoothing_power { 0.02f };

    /* Addon: Tangential Correction:
     * Helps maintain target tangential velocity
     * while in a semi-stable or stable orbit */
    float const tangential_target_velocity { World.scale_distance(500.0f) };
    constexpr static float tangential_boosting_power { 0.005f };
    constexpr static float tangential_smoothing_power { 0.01f };

    /* Addon: Planet Mass Adjustment: Helps make a semi-stable orbit circular;
     * See Navigation::apply_assistance() impl for more details */


private:
    void init_smoothing_ring_shape(sf::Vector2f const& position, float inner_radius, float outer_radius);

    std::vector<Planet>& m_planets;
    Player& m_player;

    sf::CircleShape m_smoothing_ring_inner;
    sf::CircleShape m_smoothing_ring_outer;
};
