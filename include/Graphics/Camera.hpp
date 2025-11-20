#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Window.hpp"

class Camera
{
public:
    constexpr static float follow_smoothing_power { 0.98f };

    Camera() = default;

    void update() const
    {
        auto& view = Window.get_view();
        sf::Vector2f const current_pos = view.getCenter();
        sf::Vector2f const new_pos =
                current_pos +
                (m_target - current_pos)
                * (1.0f - follow_smoothing_power);

        view.setCenter(new_pos);
    }

    void set_target(sf::Vector2f const& target) {m_target = target;}
    [[nodiscard]] sf::Vector2f const& get_target() const {return m_target;}

private:
    sf::Vector2f m_target;
};

using Camera_t = Camera;
extern Camera_t Camera;