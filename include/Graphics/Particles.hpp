#pragma once

#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Graphics/World.hpp"
#include "Math/Random.hpp"

struct Particle
{
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float lifetime { 0.0f };
};

class ParticleEmitter
{
public:
    ParticleEmitter() = default;

    /* Visual Parameters */
    ufd const param_visual_particle_radius_dist {1.0f, 4.0f};
    ufd const param_visual_particle_color_hue_dist {0.0f, 359.0f};
    constexpr static float param_visual_particle_color_whiteness { 30.0f };
    constexpr static float param_visual_particle_color_blackness { 0.0f };

    /* Emitter Parameters */
    ufd const param_emit_particle_spread_angle_dist { 0.0f, 359.0f };
    std::pair<ufd, ufd> const param_emit_particle_velocity_dist {
        ufd { World::scale_distance(100.f), World::scale_distance(500.0f) }, // x
        ufd { World::scale_distance(100.f), World::scale_distance(500.0f) }, // y
    };
    ufd const param_emit_particle_lifetime_dist { 0.5f, 2.0f }; // seconds
    constexpr static sf::Vector2f param_emit_particle_acceleration { 0.0f , World::scale_distance(100.0f) };

    void update();
    void draw() const;

    void emit(uint32_t count, sf::Vector2f const& position);

    [[nodiscard]] bool is_active() const { return !m_particles.empty(); }

private:
    std::vector<Particle> m_particles;
    Random m_random;
};

using ParticleEmitter_t = ParticleEmitter;
extern ParticleEmitter_t ParticleEmitter;