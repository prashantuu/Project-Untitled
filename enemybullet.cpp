#include "EnemyBullet.h"

EnemyBullet::EnemyBullet()
{
    speed = 250.f;
    alive = true;

    shape.setSize({8.f, 8.f});
    shape.setFillColor(sf::Color::Red);
}

void EnemyBullet::update(float deltaTime)
{
    shape.move(direction * speed * deltaTime);
}

void EnemyBullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}