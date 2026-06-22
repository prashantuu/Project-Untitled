#include "Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Vector2f position)
{
    body.setSize(sf::Vector2f(40.f, 40.f));
    body.setFillColor(sf::Color(150, 0, 0)); // hell red

    body.setPosition(position);

    speed = 100.f;
    health = 3;
}
void Enemy::update(float deltaTime, sf::Vector2f playerPos)
{
    sf::Vector2f direction = playerPos - body.getPosition();

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0)
        direction /= length;

    body.move(direction * speed * deltaTime);
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::FloatRect Enemy::getBounds() const
{
    return body.getGlobalBounds();
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
}

bool Enemy::isDead() const
{
    return health <= 0;
}
