#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const sf::Texture& texture)
    : shape(texture)
{
    hp = 2;
    alive = true;
    shootTimer = 0.f;

    // Resize enemy
    shape.setScale({0.05f, 0.05f});

    // Center the origin
    sf::FloatRect bounds = shape.getLocalBounds();

    shape.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
}

void Enemy::update(sf::Vector2f target, float speed, float deltaTime)
{
    sf::Vector2f dir = target - shape.getPosition();

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
    {
        dir /= len;

        // Rotate enemy to face the target
        float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;

        shape.setRotation(sf::degrees(angle));

        // Move enemy
        shape.move(dir * speed * deltaTime);
    }
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}