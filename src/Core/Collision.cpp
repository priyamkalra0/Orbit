#include "Core/Collision.hpp"
#include "Core/Level.hpp"
#include "Math/Vector2.hpp"

bool Collision::poll_collision(Player const& player)
{
    auto const& planets = Level.get_planets();
    return std::any_of(
        planets.begin(),
        planets.end(),
        [&player](auto const& planet)
        { return intersects(player, planet.get_info()); }
    );
}

bool Collision::intersects(Player const& player, PlanetInfo const& planet)
{
    auto const& player_shape = player.get_shape();
    auto const& player_transform = player_shape.getTransform();

    // Get player's vertices in world coordinates
    size_t const vertex_count = player_shape.getPointCount();
    std::vector<sf::Vector2f> player_vertices;
    player_vertices.reserve(vertex_count);
    for (size_t idx = 0; idx < vertex_count; ++idx)
        player_vertices.push_back(
            player_transform * player_shape.getPoint(idx)
        );

    // Check if any player vertex is inside the planet's circle
    return std::any_of(
        player_vertices.begin(),
        player_vertices.end(),
        [&planet](auto const& v)
        {return (v - planet.position).length() < planet.radius;}
    );
}