#pragma once

#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"

struct NavigationContext
{
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

    void bind(Player& player) {  m_player = &player; }
    void load_context() { m_context.emplace(make_context()); }

    constexpr static float G { 10.0f };

    void draw() const;
    void update();

    [[nodiscard]] NavigationContext const& get_context() const
    {
        assert(has_context());
        return *m_context;
    }

    [[nodiscard]] bool has_context() const { return m_context.has_value(); }

private:
    [[nodiscard]] NavigationContext make_context() const;

    std::optional<NavigationContext> m_context;
    Player* m_player { nullptr };

};

using Navigation_t = Navigation;
extern Navigation_t Navigation;