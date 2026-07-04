#include "player/Player.h"
#include <cmath>
#include <algorithm>

const float Player::SPEED = 200.f;
const sf::Time Player::INVINCIBILITY_DURATION = sf::seconds(0.5f);

Player::Player()
    : m_shape({40.f, 20.f})
    , m_currentHealth(100)
    , m_maxHealth(100)
    , m_isInvincible(false)
    , m_invincibilityTimer(sf::Time::Zero)
{
    m_shape.setOrigin({20.f, 10.f});

    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition({400.f, 300.f});
}

void Player::update(sf::Time deltaTime, const sf::RenderWindow& window)
{
    handleMovement(deltaTime);
    handleRotation(window);
    updateInvincibility(deltaTime);

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

void Player::updateInvincibility(sf::Time deltaTime)
{
    if (!m_isInvincible)
        return;

    m_invincibilityTimer += deltaTime;

    if (m_invincibilityTimer >= INVINCIBILITY_DURATION)
    {
        m_isInvincible = false;
        m_invincibilityTimer = sf::Time::Zero;
    }
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
    return m_weapon.checkHit(targetBounds);
}

sf::Vector2f Player::getPosition() const
{
    return m_shape.getPosition();
}

sf::FloatRect Player::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Player::constrainToWorld(sf::Vector2f worldSize)
{
    sf::Vector2f position = m_shape.getPosition();

    position.x = std::clamp(position.x, 0.f, worldSize.x);
    position.y = std::clamp(position.y, 0.f, worldSize.y);

    m_shape.setPosition(position);
}

void Player::takeDamage(int amount)
{
    // EnemyManager doesn't need to know invincibility exists — it just
    // calls takeDamage(), and Player decides whether it actually applies.
    if (m_isInvincible)
        return;

    m_currentHealth -= amount;
    m_currentHealth = std::max(m_currentHealth, 0);

    m_isInvincible = true;
    m_invincibilityTimer = sf::Time::Zero;
}

int Player::getHealth() const
{
    return m_currentHealth;
}

bool Player::isAlive() const
{
    return m_currentHealth > 0;
}