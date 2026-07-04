#include "player/Player.h"
#include <cmath>
#include <algorithm>

const float Player::SPEED = 200.f;
const sf::Time Player::INVINCIBILITY_DURATION = sf::seconds(0.5f);

Player::Player()
    : m_sprite(m_texture)
    , m_currentHealth(100)
    , m_maxHealth(100)
    , m_isInvincible(false)
    , m_invincibilityTimer(sf::Time::Zero)
{
    // NOTE: replace with your actual downloaded player sprite.
    m_texture.loadFromFile("assets/sprites/player/player.png");

    sf::Vector2u size = m_texture.getSize();
    m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});

    m_sprite.setPosition({400.f, 300.f}); // overridden by Game via setPosition()
}

void Player::update(sf::Time deltaTime, const sf::RenderWindow& window)
{
    handleMovement(deltaTime);
    handleRotation(window);
    updateInvincibility(deltaTime);

    m_weapon.update(deltaTime, m_sprite.getPosition(), m_sprite.getRotation());
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

    m_sprite.move(movement * deltaTime.asSeconds());
}

void Player::handleRotation(const sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f playerPos = m_sprite.getPosition();
    sf::Vector2f direction = mouseWorld - playerPos;

    float angleRadians = std::atan2(direction.y, direction.x);
    m_sprite.setRotation(sf::radians(angleRadians));
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
    window.draw(m_sprite);
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
    return m_sprite.getPosition();
}

sf::FloatRect Player::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

void Player::setPosition(sf::Vector2f position)
{
    m_sprite.setPosition(position);
}

void Player::constrainToWorld(sf::Vector2f worldSize)
{
    sf::Vector2f position = m_sprite.getPosition();

    position.x = std::clamp(position.x, 0.f, worldSize.x);
    position.y = std::clamp(position.y, 0.f, worldSize.y);

    m_sprite.setPosition(position);
}

bool Player::takeDamage(int amount)
{
    if (m_isInvincible)
        return false;

    m_currentHealth -= amount;
    m_currentHealth = std::max(m_currentHealth, 0);

    m_isInvincible = true;
    m_invincibilityTimer = sf::Time::Zero;

    return true;
}

int Player::getHealth() const
{
    return m_currentHealth;
}

int Player::getMaxHealth() const
{
    return m_maxHealth;
}

bool Player::isAlive() const
{
    return m_currentHealth > 0;
}