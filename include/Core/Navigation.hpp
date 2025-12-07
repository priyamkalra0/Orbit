#pragma once

#include <utility>
#include <optional>
#include "Core/Game.hpp"
#include "Entity/Planet.hpp"
#include "Entity/Orbit.hpp"
#include "Entity/Player.hpp"
#include "Math/Vector2.hpp"

struct NavigationContext
{
    /* Player's velocity components
     * relative to target planet */
    sf::Vector2f player_radial_v;
    sf::Vector2f player_tangent_v;

    /* Player's error relative to current
     * target orbit */
    float const player_error;

    /* References to the target planet
     * and target orbit; towards which
     * the player is being pulled.
     * This orbit will always be ON.
     */
    Planet& target_planet;
    Orbit& target_orbit;

    /* References to the nearest planet
     * and nearest orbit;
     * These may or may not be ON.
     * Sometimes we want the nearest orbit
     * even if it's turned off.
     */
    Planet& nearest_planet;
    Orbit& nearest_orbit;

    /* References to the previous planet
     * and previous orbit;
     * These most likely will be OFF;
     * but we do not care about that.
     */
    Planet& previous_planet;
    Orbit& previous_orbit;
};


class Navigation
{
public:
    Navigation() = default;

    void force_reload() { m_context.emplace(make_context()); }

    constexpr static float G { 10.0f };

    void draw() const;
    void update();

    [[nodiscard]] NavigationContext const& get_context() const
    {
        assert(has_context());
        return *m_context;
    }

    [[nodiscard]] bool has_context() const { return m_context.has_value(); }

    void release_player_from_orbit();

private:
    [[nodiscard]] NavigationContext make_context() const;

    [[nodiscard]] std::pair<sf::Vector2f, sf::Vector2f> ctx_get_velocity_components() const;
    Planet& ctx_get_previous_planet(Planet& current_target_ref) const;

    std::optional<NavigationContext> m_context;
    Player* m_player { nullptr };

};

using Navigation_t = Navigation;
extern Navigation_t Navigation;