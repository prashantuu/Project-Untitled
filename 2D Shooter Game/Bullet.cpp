#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f position, sf::Vector2f target)
{
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);

    speed = 1000.f;

    // direction = target - position
    sf::Vector2f diff = target - position;

    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    direction = diff / length; // normalize
}
void Bullet::update(float deltaTime)
{
    shape.move(direction * speed * deltaTime);
}
void Bullet::render(sf::RenderWindow& window)
{
    window.draw(shape);
}
sf::FloatRect Bullet::getBounds() const
{
    return shape.getGlobalBounds();
}