#pragma once

#include <optional>
#include <SFML/Graphics.hpp>

class Window
{
public:
    Window(std::string const& title, sf::Vector2u const& initial_size, sf::Vector2u const& internal_resolution)
        : m_render_window{
            sf::VideoMode{{initial_size.x, initial_size.y}},
            title, sf::State::Windowed,
            sf::ContextSettings{0, 0, 16} // 16x anti-aliasing
        },
          m_internal_resolution{internal_resolution},
          m_framerate_limit{0}
    {
        set_framerate_limit(120);
        m_view.setSize({
            static_cast<float>(m_internal_resolution.x),
            static_cast<float>(m_internal_resolution.y)
        });
        m_view.setCenter({
            m_view.getSize().x / 2.0f,
            m_view.getSize().y / 2.0f
        });
        set_view(m_view);
    }

    void handle_resize_event(sf::Event::Resized const& event)
    {
        sf::Vector2u const& size = event.size;
        auto const window_width = static_cast<float>(size.x);
        auto const window_height = static_cast<float>(size.y);

        float const world_aspect_ratio =
                static_cast<float>(m_internal_resolution.x)
                / static_cast<float>(m_internal_resolution.y);
        float const window_aspect_ratio = window_width / window_height;

        float viewport_width = 1.0f;
        float viewport_height = 1.0f;
        float viewport_x = 0.0f;
        float viewport_y = 0.0f;

        if (window_aspect_ratio > world_aspect_ratio)
        {
            // Window is wider than world, pillarbox
            viewport_width = world_aspect_ratio / window_aspect_ratio;
            viewport_x = (1.0f - viewport_width) / 2.0f;
        }
        else if (window_aspect_ratio < world_aspect_ratio)
        {
            // Window is taller than world, letterbox
            viewport_height = window_aspect_ratio / world_aspect_ratio;
            viewport_y = (1.0f - viewport_height) / 2.0f;
        }

        m_view.setViewport({
            {viewport_x, viewport_y},
            {viewport_width, viewport_height}
        });
        set_view(m_view);
    }

    void set_view(sf::View const& view)
    {
        m_render_window.setView(view);
    }

    std::optional<sf::Event> poll_event()
    {
        return m_render_window.pollEvent();
    }

    bool is_open() const
    {
        return m_render_window.isOpen();
    }

    void clear()
    {
        m_render_window.clear();
    }

    void draw(sf::Drawable const& drawable)
    {
        m_render_window.draw(drawable);
    }

    void display()
    {
        m_render_window.display();
    }

    sf::RenderWindow& get_render_window()
    {
        return m_render_window;
    }

    uint32_t get_framerate_limit() const { return m_framerate_limit; }
    void set_framerate_limit(uint32_t const framerate_limit)
    {
        m_render_window.setFramerateLimit(framerate_limit);
        m_framerate_limit = framerate_limit;
    }

    float get_delta_time() const { return 1.0f / static_cast<float>(m_framerate_limit); }

    ~Window()
    {
        // Automatically close; works the best.
        m_render_window.close();
    }

    sf::Vector2u const& get_internal_resolution() const { return m_internal_resolution; }
    sf::View const& get_default_view() const { return m_view; }
    sf::View const& get_current_view() const { return m_render_window.getView(); }

private:
    sf::View m_view;
    sf::RenderWindow m_render_window;
    sf::Vector2u m_internal_resolution;
    uint32_t m_framerate_limit;
};

using Window_t = Window;
extern Window_t Window;