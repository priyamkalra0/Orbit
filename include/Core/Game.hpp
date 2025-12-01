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
    void process_mouse_click(sf::Event::MouseButtonPressed const& _);
    void process_mouse_move(sf::Event::MouseMoved const& mouse);
    void process_key(sf::Event::KeyPressed const& key);

    void update();
    void render() const;

    bool m_debug_mode { false };
    Player m_player;
};
