#include "graphics/ParticleSystem.h"
#include <algorithm>
#include <cmath>
#include <ctime>

ParticleSystem::ParticleSystem()
    : m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
}

void ParticleSystem::emit(ParticleType type, sf::Vector2f position, int count)
{
    std::uniform_real_distribution<float> angleDist(0.f, 360.f);

    for (int i = 0; i < count; ++i)
    {
        Particle particle;
        particle.position = position;
        particle.lifetime = 0.f;

        float angleDegrees = angleDist(m_randomEngine);
        float angleRadians = angleDegrees * 3.14159f / 180.f;

        switch (type)
        {
            case ParticleType::Blood:
            {
                std::uniform_real_distribution<float> speedDist(40.f, 120.f);
                float speed = speedDist(m_randomEngine);
                particle.velocity = { std::cos(angleRadians) * speed, std::sin(angleRadians) * speed };
                particle.color = sf::Color(150, 0, 0);
                particle.maxLifetime = 0.4f;
                particle.size = 4.f;
                break;
            }
            case ParticleType::MuzzleFlash:
            {
                std::uniform_real_distribution<float> speedDist(20.f, 60.f);
                float speed = speedDist(m_randomEngine);
                particle.velocity = { std::cos(angleRadians) * speed, std::sin(angleRadians) * speed };
                particle.color = sf::Color(255, 200, 80);
                particle.maxLifetime = 0.1f;
                particle.size = 5.f;
                break;
            }
            case ParticleType::Smoke:
            {
                particle.velocity = { 0.f, -20.f }; // drifts upward
                particle.color = sf::Color(120, 120, 120, 180);
                particle.maxLifetime = 0.8f;
                particle.size = 6.f;
                break;
            }
            case ParticleType::Spark:
            {
                std::uniform_real_distribution<float> speedDist(80.f, 160.f);
                float speed = speedDist(m_randomEngine);
                particle.velocity = { std::cos(angleRadians) * speed, std::sin(angleRadians) * speed };
                particle.color = sf::Color(255, 240, 150);
                particle.maxLifetime = 0.25f;
                particle.size = 3.f;
                break;
            }
        }

        m_particles.push_back(particle);
    }
}

void ParticleSystem::update(sf::Time deltaTime)
{
    float dt = deltaTime.asSeconds();

    for (auto& particle : m_particles)
    {
        particle.position += particle.velocity * dt;
        particle.lifetime += dt;
    }

    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p) { return p.lifetime >= p.maxLifetime; }),
        m_particles.end());
}

void ParticleSystem::draw(sf::RenderWindow& window)
{
    for (auto& particle : m_particles)
    {
        float lifeFraction = 1.f - (particle.lifetime / particle.maxLifetime);
        sf::Color fadedColor = particle.color;
        fadedColor.a = static_cast<std::uint8_t>(255 * std::max(0.f, lifeFraction));

        sf::CircleShape shape(particle.size);
        shape.setOrigin({particle.size, particle.size});
        shape.setPosition(particle.position);
        shape.setFillColor(fadedColor);

        window.draw(shape);
    }
}