#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Orbit
{
public:
    Orbit(sf::Vector2f const& position, float radius);

    void draw() const;

    [[nodiscard]] float get_radius() const { return m_radius; };

    void turn_on() { m_state = true;}
    void turn_off() { m_state = false;}
    void toggle() { m_state = !m_state;}

    [[nodiscard]] bool is_on() const { return m_state; }

private:
    bool m_state;
    float m_radius;
    std::vector<sf::CircleShape> m_rings;
};
