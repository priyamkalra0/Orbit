#include "Core/Navigation.hpp"
#include "Core/Level.hpp"
#include "Graphics/Window.hpp"
#include "Math/Vector2.hpp"

Planet& Navigation::get_active_planet() const
{
    Planet* active_planet { nullptr };

    float smallest_distance { -1 };
    for (auto& planet : Level.get_planets())
    {
        if (!planet.get_orbit().is_on()) continue;
        float const d { m_player->get_distance_squared(planet.get_info()) };
        if (smallest_distance > 0.0f && d > smallest_distance) continue;
        smallest_distance = d;
        active_planet = &planet;
    }

    return *active_planet;
}

void Navigation::draw() const { }

void Navigation::update() { }