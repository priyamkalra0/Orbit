#include "Graphics/Particles.hpp"
#include "Core/Collision.hpp"
#include "Core/Navigation.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Color.hpp"

void ParticleEmitter::update()
{
    if (!is_active()) return;

    float const dt = Window.get_delta_time();
    auto& ctx = Navigation.get_context();

    for (auto p = m_particles.begin(); p != m_particles.end(); )
    {
        p->lifetime -= dt;

        if (
            p->lifetime <= 0.0f ||
            Collision::intersects(ctx.nearest_planet.get_info(), p->shape)
        ) {
            p = m_particles.erase(p); continue;
        }

        /* Simple acceleration:
         * We do not store the position in the particle
         * struct; so we just use the sf::shape for that.
         * This should be fine since these
         * shapes have a limited lifetime anyways,
         * and only managed by this method
         */
        p->velocity += param_emit_particle_acceleration * dt;
        p->shape.setPosition(
            p->shape.getPosition()
            + p->velocity * dt
        );

        /* Fade out */
        float const ratio = p->lifetime / m_random.get(param_emit_particle_lifetime_dist);
        sf::Color color = p->shape.getFillColor();
        color.a = static_cast<uint8_t>(255 * ratio);
        p->shape.setFillColor(color);

        ++p;
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
        auto const angle = sf::degrees(m_random.get(param_emit_particle_spread_angle_dist));
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
