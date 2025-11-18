#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Window.hpp"

class Camera
{
public:
    constexpr static float follow_smoothing_power { 0.99f };

    explicit Camera()
    {
        auto const& internal_resolution = Window.get_internal_resolution();
        m_view.setSize({
            static_cast<float>(internal_resolution.x),
            static_cast<float>(internal_resolution.y)
        });
        m_view.setCenter({
            m_view.getSize().x / 2.0f,
            m_view.getSize().y / 2.0f
        });
    }

    void follow(sf::Vector2f const& position)
    {
        sf::Vector2f const current_pos = m_view.getCenter();
        sf::Vector2f const new_pos =
                current_pos +
                (position - current_pos)
                * (1.0f - follow_smoothing_power);

        m_view.setCenter(new_pos);
    }

    void apply() const
    {
        Window.set_view(m_view);
    }

private:
    sf::View m_view;
};

using Camera_t = Camera;
extern Camera_t Camera;