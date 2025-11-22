#include <optional>

#include "Game.hpp"
#include "Level.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/World.hpp"
#include "Graphics/Camera.hpp"

Window_t Window {
    "Orbit",
    {1280, 720}, /* smaller window = easier to manage */
    {2560, 1440}
};
World_t World {{1920, 1080}, {2560, 1440}};
Camera_t Camera;
Level_t Level;

Game::Game()
    : m_navigation { m_player }
{
    Level.generate(m_player);
    reset_player(); // fix player to planet closest to origin
}

void Game::run()
{
    while (Window.is_open())
    {
        bool const exit_signal { process_events() };
        if (exit_signal) break;

        update();
        render();
    }
}

bool Game::process_events()
{
    while (const std::optional event { Window.poll_event() })
    {
        if (event->is<sf::Event::Closed>()) return true;

        if (auto const* resized {event->getIf<sf::Event::Resized>()})
            Window.handle_resize(resized->size);

        if (auto const* key {event->getIf<sf::Event::KeyPressed>()})
            process_input(*key);
    }
    return false;
}

void Game::process_input(sf::Event::KeyPressed const& key)
{
    if (key.code == sf::Keyboard::Key::Space) // toggle orbit
    {
        Planet& active_planet { m_navigation.get_active_planet() };
        for (auto& planet : Level.get_planets()) planet.get_orbit().turn_on(); // turn on everything else
        return active_planet.get_orbit().toggle(); // turn off active planet's orbit
    }

    /* Debug Cheats */
    if (key.code == sf::Keyboard::Key::D) // toggle debug
        {m_debug_mode = !m_debug_mode; return;}

    if (!m_debug_mode) return;

    if (key.code == sf::Keyboard::Key::R)
        return reset_player();

    if (key.code == sf::Keyboard::Key::I)
        return m_player.set_velocity(-1.0f * m_player.get_velocity()); // invert velocity
}

void Game::update()
{
    m_navigation.update();

    Planet& planet { m_navigation.get_active_planet() };
    Orbit const& orbit { planet.get_orbit() };

    orbit.apply_force(m_player);
    m_player.update();

    Camera.update();

    /* Follow player from the next frame
     * if it is far outside the orbit of any planet */
    Camera.set_target(planet.get_position());
    if (m_player.is(PlayerState::FarOutsideOrbit))
        Camera.set_target(m_player.get_position());
}

void Game::render() const
{
    Window.clear();

    for (auto const& planet : Level.get_planets())
        planet.draw();

    if (m_debug_mode) m_navigation.draw();
    m_player.draw();

    Window.display();
}

void Game::reset_player()
{
    auto const& planet { m_navigation.get_active_planet() };
    float const r { planet.get_orbit().get_radius() };
    m_player.set_position(planet.get_position() - sf::Vector2f{0, r});
    m_player.set_velocity({World.scale_x(m_navigation.tangential_target_velocity), 0});
}
