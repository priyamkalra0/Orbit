#include "Graphics/Window.hpp"
#include "Entity/Player.hpp"

Player::Player(sf::Vector2f const& position, sf::Vector2f const& initial_velocity)
    : m_position{position},
      m_acceleration{0.0f, 0.0f}
{
    // Verlet integration needs the previous position
    float const dt = Window.get_delta_time();
    m_previous_position = m_position - initial_velocity * dt;

    m_shape.setPointCount(3);
    m_shape.setPoint(0, {0.0f, -20.0f});
    m_shape.setPoint(1, {10.0f, 10.0f});
    m_shape.setPoint(2, {-10.0f, 10.0f});
    m_shape.setOrigin({0.0f, 0.0f});
    m_shape.setPosition(m_position);
    m_shape.setFillColor(sf::Color::Red);
}

void Player::draw() const
{
    Window.draw(m_shape);
}

void Player::accelerate(sf::Vector2f const& force)
{
    m_acceleration += force; // a = f/m = f (player has unit mass)
}

void Player::update()
{
    float const dt = Window.get_delta_time();

    // Verlet integration
    sf::Vector2f const current_position{ m_position };
    m_position = 2.0f * current_position - m_previous_position + m_acceleration * (dt * dt);
    m_previous_position = current_position;
    m_acceleration = {0.0f, 0.0f};

    // Sync the drawable shape
    m_shape.setPosition(m_position);

    // Calculate rotation based on velocity
    sf::Vector2f const current_velocity = get_velocity();
    if (current_velocity.lengthSquared() <  0.0f) return;
    float const angle_radians = std::atan2(current_velocity.y, current_velocity.x);
    float const angle_degrees = angle_radians * 180.0f / M_PI;
    m_shape.setRotation(sf::degrees(angle_degrees + 90.0f));
}

sf::Vector2f Player::get_velocity() const
{
    float const dt = Window.get_delta_time();
    return (m_position - m_previous_position) / dt;
}

sf::Vector2f Player::get_distance_vec(PlanetInfo const& planet) const
{
    return get_position() - planet.Position;
}

float Player::get_distance(PlanetInfo const& planet) const
{
    return get_distance_vec(planet).length();
}

float Player::get_distance_squared(PlanetInfo const& planet) const
{
    return get_distance_vec(planet).lengthSquared();
}

void Player::set_velocity(sf::Vector2f const& new_velocity)
{
    // Since we are using Verlet Integration internally,
    // all we need to do is overwrite m_previous_position
    float const dt = Window.get_delta_time();
    m_previous_position = m_position - new_velocity * dt;
    m_acceleration = {0.0f, 0.0f};
}

sf::Vector2f Player::get_radial_velocity_vector(PlanetInfo const& planet) const
{
    sf::Vector2f const radial_direction = get_distance_vec(planet).normalized();
    return get_velocity().projectedOnto(radial_direction);
}

sf::Vector2f Player::get_tangential_velocity_vector(PlanetInfo const& planet) const
{
    return get_velocity() - get_radial_velocity_vector(planet);
}

void Player::set_position(sf::Vector2f const& position)
{
    m_position = position;
    m_previous_position = position;
}
