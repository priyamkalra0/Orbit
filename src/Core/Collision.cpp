#include "Core/Collision.hpp"
#include "Core/Level.hpp"
#include "Math/Vector2.hpp"

bool Collision::with_any_planet(sf::Shape const& shape)
{
    auto const& planets { Level.get_planets() };
    return std::any_of(
        planets.begin(),
        planets.end(),
        [&shape](auto const& planet)
        { return with_planet(planet.get_info(), shape); }
    );
}

bool Collision::with_planet(PlanetInfo const& planet, sf::Shape const& shape)
{
    auto const& shape_transform { shape.getTransform() };

    // Get shape's vertices in world coordinates
    size_t const vertex_count { shape.getPointCount() };
    std::vector<sf::Vector2f> shape_vertices;
    shape_vertices.reserve(vertex_count);
    for (size_t idx = 0; idx < vertex_count; ++idx)
        shape_vertices.push_back(
            shape_transform * shape.getPoint(idx)
        );

    // Check if any shape vertex is inside the planet's circle
    return std::any_of(
        shape_vertices.begin(),
        shape_vertices.end(),
        [&planet](auto const& v)
        {return (v - planet.position).length() < planet.radius;}
    );
}