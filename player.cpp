#include "Player.h"
#include <cmath>

Player::Player(const sf::Texture& texture)
    : sprite(texture)
{
    speed = 300.f;
    angle = 0.f;
    walkTime = 0.f;
    moving = false;

    // Scale player
    sprite.setScale({0.5f, 0.5f});

    // Get sprite bounds
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Set origin near the feet
    sprite.setOrigin({
        bounds.size.x / 2.f,
        bounds.size.y * 0.85f
    });
}

void Player::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Player::move(sf::Vector2f movement)
{
    moving = (movement.x != 0.f || movement.y != 0.f);

    sprite.move(movement);
}
void Player::rotate(sf::Vector2f mousePos)
{
    angle = std::atan2(
        mousePos.y - sprite.getPosition().y,
        mousePos.x - sprite.getPosition().x
    ) * 180.f / 3.14159265f;

    sprite.setRotation(sf::degrees(angle));
}

sf::Sprite& Player::getSprite()
{
    return sprite;
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect Player::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

float Player::getAngle() const
{
    return angle;
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}