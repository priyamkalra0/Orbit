#include <optional>
#include <vector>

#include "Game.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/World.hpp"

Window_t Window{"Orbit", {1280, 720}, {1920, 1080}};
World_t World{{1280, 720}, {1920, 1080}};

Game::Game()
    : m_player {
        World.scale_position({0.0f, 220.0f}),
        World.scale_velocity({0.75f * m_navigation.tangential_target_velocity, 0.0f})
    },
    m_navigation { m_planets, m_player }
{
    /* Planet Generation */
    constexpr uint32_t planet_count { 3 };
    m_planets.reserve(planet_count);

    float const v_target_sq = m_player.get_velocity().lengthSquared();

    // Planet 1 (Far Left)
    float const p1_orbit_r = World.scale_distance(170.0f);
    float const p1_mass { (v_target_sq * p1_orbit_r) / Navigation::G };
    m_planets.emplace_back(
        World.scale_position({250.0f, 500.0f}),
        p1_mass,
        World.scale_distance(50.0f),
        p1_orbit_r
    );

    // Planet 2 (Mid-Right)
    float const p2_orbit_r = World.scale_distance(200.0f);
    float const p2_mass { (v_target_sq * p2_orbit_r) / Navigation::G };
    m_planets.emplace_back(
        World.scale_position({650.0f, 200.0f}),
        p2_mass,
        World.scale_distance(70.0f),
        p2_orbit_r
    );

    // Planet 3 (Far Right)
    float const p3_orbit_r = World.scale_distance(150.0f);
    float const p3_mass { (v_target_sq * p3_orbit_r) / Navigation::G };
    m_planets.emplace_back(
        World.scale_position({1100.0f, 400.0f}),
        p3_mass,
        World.scale_distance(30.0f),
        p3_orbit_r
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
    while (const std::optional<sf::Event> event = Window.poll_event())
    {
        if (event->is<sf::Event::Closed>()) return true;

        if (auto const* resized {event->getIf<sf::Event::Resized>()})
            Window.handle_resize_event(*resized);

        if (auto const* key {event->getIf<sf::Event::KeyPressed>()})
            process_input(*key);
    }
    return false;
}

void Game::process_input(sf::Event::KeyPressed const& key)
{
    if (key.code == sf::Keyboard::Key::R)
        return reset_player();

    if (key.code == sf::Keyboard::Key::I)
        return m_player.set_velocity(-1.0f * m_player.get_velocity()); //debug: invert velocity

    if (key.code == sf::Keyboard::Key::Space)
    {
        Planet& active_planet = m_navigation.get_active_planet();
        for (auto& planet : m_planets) planet.get_orbit().turn_on(); // turn on everything else
        return active_planet.get_orbit().toggle(); // turn off active planet's orbit
    }
}

void Game::update()
{
    m_navigation.apply_assistance();

    Planet& active_planet = m_navigation.get_active_planet();

    Orbit const& orbit = active_planet.get_orbit();
    orbit.apply_force(active_planet.get_info(), m_player);

    m_player.update();
}

void Game::render() const
{
    Window.clear();

    for (auto const& planet : m_planets)
        planet.draw();

    m_navigation.draw();
    m_player.draw();

    Window.display();
}

void Game::reset_player()
{
    auto const& planet = m_navigation.get_active_planet();
    float const r = planet.get_orbit().get_radius();
    m_player.set_position(planet.get_position() - sf::Vector2f{0, r});
    m_player.set_velocity({World.scale_x(m_navigation.tangential_target_velocity), 0});
}
