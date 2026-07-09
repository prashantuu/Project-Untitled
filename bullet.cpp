#include "Bullet.h"
Bullet::Bullet(const sf::Texture& texture)
    : shape(texture)
{
    speed = 500.f;
    alive = true;

    shape.setScale({0.08f, 0.08f});

    sf::FloatRect bounds = shape.getLocalBounds();

    shape.setOrigin({
        bounds.size.x / 2.f,
        bounds.size.y / 2.f
    });
}
void Bullet::update(float deltaTime)
{
    shape.move(direction * speed * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}