#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

enum class ParticleType
{
    Blood,
    MuzzleFlash,
    Smoke,
    Spark
};

// Particles are simple colored shapes, not textured sprites — this
// avoids needing new art assets just for visual effects, and is
// the standard lightweight approach for this kind of effect.
struct Particle
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float size;
};

class ParticleSystem
{
public:
    ParticleSystem();

    void emit(ParticleType type, sf::Vector2f position, int count = 1);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Particle> m_particles;
    std::mt19937 m_randomEngine;
};