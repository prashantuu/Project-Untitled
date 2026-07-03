#include "player/Player.h"

const float Player::SPEED = 200.f;

Player::Player()
    : m_shape(20.f)
{
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition({400.f, 300.f});
}

void Player::update(sf::Time deltaTime)
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

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}