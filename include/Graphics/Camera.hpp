#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Window.hpp"

class Camera
{
public:
    constexpr static float follow_smoothing_power { 0.99f };

    Camera() = delete;

    static void follow(sf::Vector2f const& position)
    {
        auto& view = Window.get_view();
        sf::Vector2f const current_pos = view.getCenter();
        sf::Vector2f const new_pos =
                current_pos +
                (position - current_pos)
                * (1.0f - follow_smoothing_power);

        view.setCenter(new_pos);
    }
};