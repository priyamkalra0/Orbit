#include <optional>
#include <vector>

#include "Game.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/World.hpp"

Window_t Window{"Orbit", {1280, 720}, {1920, 1080}};
World_t World{{1280, 720}, {1920, 1080}};

Game::Game()
{
    /* Planet Configuration */
    constexpr uint32_t planet_count { 3 };
    m_planets.reserve(planet_count);

    // Planet 1 (Far Left)
    m_planets.emplace_back(
        World.scale_position({250.0f, 500.0f}),
        1000.0f,
        World.scale_distance(50.0f),
        World.scale_distance(200.0f)
    );

    // Planet 2 (Mid-Right)
    m_planets.emplace_back(
        World.scale_position({650.0f, 200.0f}),
        500.0f,
        World.scale_distance(70.0f),
        World.scale_distance(250.0f)
    );

    // Planet 3 (Far Right)
    m_planets.emplace_back(
        World.scale_position({1100.0f, 400.0f}),
        500.0f,
        World.scale_distance(30.0f),
        World.scale_distance(150.0f)
    );
}

void Game::run()
{
    while (Window.is_open())
    {
        bool const exit_event = process_events();
        if (exit_event) break;

        update();
        render();
    }
}

bool Game::process_events()
{
    while (const std::optional event = Window.poll_event())
    {
        if (event->is<sf::Event::Closed>()) return true;
        if (event->is<sf::Event::Resized>())
            Window.handle_resize_event(*event->getIf<sf::Event::Resized>());
    }
    return false;
}

void Game::update()
{
}

void Game::render() const
{
    Window.clear();

    for (auto const& planet : m_planets)
        planet.draw();

    Window.display();
}