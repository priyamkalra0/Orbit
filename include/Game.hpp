#pragma once

#include <vector>

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

    Navigation m_navigation;
    std::vector<Planet> m_planets;
    Player m_player;
};
