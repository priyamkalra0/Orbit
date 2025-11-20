#pragma once

#include "Navigation.hpp"
#include "Entity/Planet.hpp"
#include "Entity/Player.hpp"

class Game
{
public:
    Game();
    void run();

private:
    bool process_events();
    void process_input(sf::Event::KeyPressed const& key);

    void update();
    void render() const;

    void reset_player();

    constexpr static bool m_debug_mode = true;
    Navigation m_navigation;
    Player m_player;
};
