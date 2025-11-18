#pragma once

class Game
{
public:
    Game();
    void run();

private:
    bool process_events();
    void update();
    void render();
};
