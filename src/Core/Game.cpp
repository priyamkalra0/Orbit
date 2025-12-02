#include <optional>

#include "Core/Game.hpp"
#include "Core/Level.hpp"
#include "Core/Navigation.hpp"
#include "Core/Assist.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Particles.hpp"
#include "Math/Vector2.hpp"

/* Graphics Managers */
Window_t Window;
Camera_t Camera;
ParticleEmitter_t ParticleEmitter;

/* Core Managers */
Navigation_t Navigation;
Assist_t Assist;
Level_t Level;

/* Game Manager */
Game_t Game;

Game::Game()
{
    Level.generate();
    Navigation.update();
    m_player.reset(); // fix player to planet closest to origin
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

        if (auto const* resized { event->getIf<sf::Event::Resized>() })
            Window.handle_resize(resized->size);

        if (auto const* key { event->getIf<sf::Event::KeyPressed>() })
            process_key(*key);

        if (auto const* mouse { event->getIf<sf::Event::MouseMoved>() })
            process_mouse_move(*mouse);

        if (auto const* mouse { event->getIf<sf::Event::MouseButtonPressed>() })
            process_mouse_click(*mouse);
    }
    return false;
}

void Game::process_key(sf::Event::KeyPressed const& key)
{
    if (key.code == sf::Keyboard::Key::Space) // release player
        Navigation.release_player_from_orbit();

    /* Debug Cheats */
    if (key.code == sf::Keyboard::Key::D) // toggle debug
        { m_debug_mode = !m_debug_mode; return; }

    if (!m_debug_mode) return;

    if (key.code == sf::Keyboard::Key::R)
        return m_player.reset();

    if (key.code == sf::Keyboard::Key::I)
        return m_player.invert_velocity();
}

void Game::process_mouse_click(sf::Event::MouseButtonPressed const& _)
{
    /* Toggles camera mode */
    if (Camera.is_locked())
    {
        Camera.unlock();
        return Camera.set_follow_smoothing_power( /* Player follow mode */
            Camera::param_player_follow_smoothing_power
        );
    }

    Camera.set_follow_smoothing_power( /* Seek mode */
        Camera::param_seek_follow_smoothing_power
    );
    Camera.lock();
}

void Game::process_mouse_move(sf::Event::MouseMoved const& mouse)
{
    /* Handles camera's seek mode */
    if (!Camera.is_locked()) return; // Seek mode OFF.

    auto const& position = \
        Window.map_pixel_to_coords(mouse.position);

    Camera.unlock();
    Camera.set_target(position);
    Camera.lock();
}

void Game::update()
{
    if (ParticleEmitter.is_active())
        /* Block the update loop
         * while the particle emitter is active
         * FIXME: process_input() is still being called */
        return ParticleEmitter.update();


    Navigation.update();
    Assist.update();

    auto& ctx = Navigation.get_context();
    auto& target_orbit = ctx.target_orbit;

    target_orbit.update();
    m_player.update();

    Camera.update();

    /* Follow player from the next frame
     * if it is far outside the orbit of any planet */
    Camera.set_target(target_orbit.get_origin());
    if (m_player.is(PlayerState::FarOutsideOrbit))
        Camera.set_target(m_player.get_position());
}

void Game::render() const
{
    Window.clear();
    ParticleEmitter.draw();

    for (auto const& planet : Level.get_planets())
        planet.draw();

    if (m_debug_mode) Assist.draw();
    m_player.draw();

    Window.display();
}