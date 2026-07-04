#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    // Same reasoning as Bullet: takes a texture reference owned by
    // EnemyManager rather than owning its own, since Enemy instances
    // live in a std::vector that can reallocate.
    Enemy(sf::Texture& texture, sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime, sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

private:
    void handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition);

private:
    sf::Sprite m_sprite;

    static const float SPEED;
};