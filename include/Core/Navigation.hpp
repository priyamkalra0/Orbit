#pragma once

#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"

class Navigation
{
public:
    Navigation() = default;

    void bind(Player& player) {  m_player = &player; }

    /* TODO: Rethink the whole navigation API,
     * right now alot of boilerplate
     * needs to be done to get the most basic
     * navigation context from this class
     * See: Player::is(), Game::update()
     */

    constexpr static float G { 10.0f };

    /* Active Planet: Closest planet whose orbit is currently on */
    [[nodiscard]] Planet& get_active_planet() const;

    void draw() const;
    void update();

private:
    Player* m_player { nullptr };
};

using Navigation_t = Navigation;
extern Navigation_t Navigation;