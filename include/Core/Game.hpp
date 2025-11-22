#pragma once

#include <SFML/Window/Event.hpp>
#include "Core/Navigation.hpp"
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

    bool m_debug_mode { false };
    Navigation m_navigation;
    Player m_player;
};
