#include "Game.hpp"
#include "Graphics/Window.hpp"

Window_t Window{"Orbit", {1280, 720}, {1920, 1080}};

Game::Game()
= default;

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

void Game::render()
{
    Window.clear();

    sf::CircleShape circle;
    circle.setPosition({500.0f, 500.0f});
    circle.setRadius(100.0f);
    circle.setFillColor(sf::Color::White);
    Window.draw(circle);

    Window.display();
}