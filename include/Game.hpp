#pragma once

#include <vector>
#include "Entity/Planet.hpp"

class Game
{
public:
    Game();
    void run();

private:
    bool process_events();
    void update();
    void render() const;

    std::vector<Planet> m_planets;
};
