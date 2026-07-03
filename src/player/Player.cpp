#include "player/Player.h"
#include <cmath>

const float Player::SPEED = 200.f;

Player::Player()
    : m_shape({40.f, 20.f})
{
    // Center the origin so the player rotates around its middle,
    // not around the top-left corner.
    m_shape.setOrigin({20.f, 10.f});

    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition({400.f, 300.f});
}

void Player::update(sf::Time deltaTime, const sf::RenderWindow& window)
{
    handleMovement(deltaTime);
    handleRotation(window);
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
    // Mouse position in window (pixel) coordinates.
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);

    // Convert to world coordinates (matters once we add a camera/view).
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f playerPos = m_shape.getPosition();

    // Direction = Mouse Position - Player Position
    sf::Vector2f direction = mouseWorld - playerPos;

    // atan2 gives us the angle (in radians) between the direction vector
    // and the positive x-axis. Since our rectangle's "front" already
    // points along +x after centering the origin, we can use this
    // angle directly as the rotation.
    float angleRadians = std::atan2(direction.y, direction.x);

    m_shape.setRotation(sf::radians(angleRadians));
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}