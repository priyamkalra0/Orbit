#include "Graphics/Particles.hpp"
#include "Core/Collision.hpp"
#include "Core/Navigation.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Color.hpp"

void ParticleEmitter::update()
{
    if (!is_active()) return;

    float const dt { Window.get_delta_time() };
    auto& ctx { Navigation.get_context() };

    for (auto particle = m_particles.begin(); particle != m_particles.end(); )
    {
        particle->lifetime -= dt;

        if (
            particle->lifetime <= 0.0f ||
            Collision::intersects(ctx.nearest_planet.get_info(), particle->shape)
        ) {
            particle = m_particles.erase(particle); continue;
        }

        /* Simple acceleration:
         * We do not store the position in the particle
         * struct; so we just use the sf::shape for that.
         * This should be fine since these
         * shapes have a limited lifetime anyways,
         * and only managed by this method
         */
        particle->velocity += param_emit_particle_acceleration * dt;
        particle->shape.setPosition(
            particle->shape.getPosition()
            + particle->velocity * dt
        );

        /* Fade out */
        float const ratio {
            particle->lifetime /
            m_random.get(param_emit_particle_lifetime_dist)
        };
        sf::Color color { particle->shape.getFillColor() };
        color.a = static_cast<uint8_t>(255 * ratio);
        particle->shape.setFillColor(color);

        ++particle;
    }
}

void ParticleEmitter::draw() const
{
    if (!is_active()) return;
    for (auto const& particle : m_particles)
        Window.draw(particle.shape);
}

void ParticleEmitter::emit(uint32_t const count, sf::Vector2f const& position)
{
    m_particles.resize(count);

    for (auto& [velocity, shape, lifetime] : m_particles)
    {
        /* Emitter */
        auto const angle {sf::degrees(
            m_random.get(param_emit_particle_spread_angle_dist)
        )};
        velocity = m_random.get(param_emit_particle_velocity_dist).rotatedBy(angle);
        lifetime = m_random.get(param_emit_particle_lifetime_dist);

        /* Visual */
        shape.setPosition(position);
        shape.setRadius(m_random.get(param_visual_particle_radius_dist));
        shape.setFillColor(Color::get<Color::HWB>(
            m_random.get(param_visual_particle_color_hue_dist),
            param_visual_particle_color_whiteness,
            param_visual_particle_color_blackness
        ));
    }
}
