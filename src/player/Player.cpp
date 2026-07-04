#include "player/Player.h"
#include <cmath>

const float Player::SPEED = 200.f;

Player::Player()
    : m_shape({40.f, 20.f})
{
    m_shape.setOrigin({20.f, 10.f});

    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition({400.f, 300.f});
}

void Player::update(sf::Time deltaTime, const sf::RenderWindow& window)
{
    handleMovement(deltaTime);
    handleRotation(window);

    m_weapon.update(deltaTime, m_shape.getPosition(), m_shape.getRotation());
}

void Player::handleMovement(sf::Time deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += SPEED;

    m_shape.move(movement * deltaTime.asSeconds());
}

void Player::handleRotation(const sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f playerPos = m_shape.getPosition();
    sf::Vector2f direction = mouseWorld - playerPos;

    float angleRadians = std::atan2(direction.y, direction.x);
    m_shape.setRotation(sf::radians(angleRadians));
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    m_weapon.draw(window);
}

void Player::shoot()
{
    m_weapon.shoot();
}

bool Player::checkHit(const sf::FloatRect& targetBounds)
{
    // Player doesn't know about bullets — it just asks its weapon.
    return m_weapon.checkHit(targetBounds);
}