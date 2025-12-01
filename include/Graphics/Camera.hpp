#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Window.hpp"

class Camera
{
public:
    Camera() = default;

    constexpr static float param_player_follow_smoothing_power { 0.98f };
    constexpr static float param_seek_follow_smoothing_power { 0.985f };
    constexpr static uint32_t param_camera_lock_timeout_ms { 2000 };

    void update()
    {
        if (
            m_locked &&
            m_lock_timer.getElapsedTime().asMilliseconds()
            > param_camera_lock_timeout_ms
        ) unlock();

        auto& view { Window.get_view() };
        sf::Vector2f const current_pos { view.getCenter() };
        sf::Vector2f const new_pos {
                current_pos +
                (m_target - current_pos)
                * (1.0f - m_follow_smoothing_power)
        };

        view.setCenter(new_pos);
    }

    void set_target(sf::Vector2f const& target) { if (!m_locked) m_target = target; }
    [[nodiscard]] sf::Vector2f const& get_target() const { return m_target; }

    void set_follow_smoothing_power(float const power) { if (!m_locked) m_follow_smoothing_power = power; }
    [[nodiscard]] float get_follow_smoothing_power() const { return m_follow_smoothing_power; }

    void unlock() { m_locked = false; }
    void lock() { m_locked = true; m_lock_timer.restart(); }

    [[nodiscard]] bool is_locked() const { return m_locked; }

private:
    float m_follow_smoothing_power { param_player_follow_smoothing_power };
    bool m_locked { false };
    sf::Vector2f m_target;
    sf::Clock m_lock_timer;
};

using Camera_t = Camera;
extern Camera_t Camera;