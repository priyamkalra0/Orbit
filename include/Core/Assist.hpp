#pragma once

#include <SFML/Graphics.hpp>
#include "Core/Navigation.hpp"
#include "Entity/Player.hpp"
#include "Graphics/World.hpp"

class Assist
{
public:
    explicit Assist();

    void bind(Player& player) { m_player = &player; }

    /* Navigation Assist Parameters;
     anything related to position/velocity should
     use scaling methods from the `World` class */

    /* Planet Mass Boosting:
     * Makes the planet's pull stronger
     * while the player is outside the orbit */
    constexpr static float param_assist_planet_mass_boosting_power { 0.5f };

    /* Radial Smoothing:
     * Helps player stabilize into orbit;
     * given that they did a sufficiently good job
     * at aiming for a tangent path to the orbit
     * that is; their radial velocity was
     * under the tolerable threshold */
    float const param_assist_radial_smoothing_threshold { World.scale_distance(400.0f) };
    constexpr static float param_assist_radial_smoothing_power { 0.025f };

    float const param_assist_radial_smoothing_ring_size { World.scale_distance(125.0f) };
    std::pair<float, float> const param_assist_radial_smoothing_ring_ratio { 1, 100 }; // inner : outer
    std::pair<float, float> const param_assist_radial_smoothing_ring_region_size {
            param_assist_radial_smoothing_ring_size * param_assist_radial_smoothing_ring_ratio.first
            / (param_assist_radial_smoothing_ring_ratio.first + param_assist_radial_smoothing_ring_ratio.second),

            param_assist_radial_smoothing_ring_size * param_assist_radial_smoothing_ring_ratio.second
            / (param_assist_radial_smoothing_ring_ratio.first + param_assist_radial_smoothing_ring_ratio.second)
    }; /* inner, outer region size */


    /* Addon: Tangential Correction:
     * Helps maintain target tangential velocity
     * while in a semi-stable or stable orbit */
    float const param_assist_tangential_target_velocity { World.scale_distance(500.0f) };
    constexpr static float param_assist_tangential_boosting_power { 0.005f };
    constexpr static float param_assist_tangential_smoothing_power { 0.002f };

    /* Addon: Planet Mass Adjustment: Helps make a semi-stable orbit circular;
     * See Assist::update() impl for more details */

    void draw() const;
    void update();

private:
    Player* m_player { nullptr };

    /* Visual */
    void init_target_radius_ring(sf::Vector2f const& position, float radius);
    void init_smoothing_ring_shape(sf::Vector2f const& position);

    sf::CircleShape m_target_radius_ring;
    sf::CircleShape m_smoothing_ring_inner;
    sf::CircleShape m_smoothing_ring_outer;
};

using Assist_t = Assist;
extern Assist_t Assist;