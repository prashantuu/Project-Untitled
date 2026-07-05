#include "player/Player.h"
#include <cmath>
#include <algorithm>

const float Player::SPEED = 200.f;
const sf::Time Player::INVINCIBILITY_DURATION = sf::seconds(0.5f);

Player::Player(ResourceManager& resources)
    : m_sprite(resources.getTexture("assets/sprites/player/player_spritesheet.png"))
    , m_weapon(resources)
    // NOTE: adjust frame size/count/row to match your actual downloaded
    // sheet. Assumes 32x32 frames, idle on row 0 (4 frames), walk on
    // row 1 (6 frames) — a common layout for Kenney character sheets.
    , m_idleAnimation({32, 32}, 4, 0, sf::seconds(0.15f), true)
    , m_walkAnimation({32, 32}, 6, 1, sf::seconds(0.1f), true)
    , m_currentAnimation(&m_idleAnimation)
    , m_currentHealth(100)
    , m_maxHealth(100)
    , m_score(0)
    , m_isInvincible(false)
    , m_invincibilityTimer(sf::Time::Zero)
{
    m_sprite.setTextureRect(m_currentAnimation->getCurrentFrameRect());
    m_sprite.setOrigin({16.f, 16.f});

    m_sprite.setPosition({400.f, 300.f}); // overridden by Game via setPosition()
}

void Player::update(sf::Time deltaTime, const sf::RenderWindow& window)
{
    bool isMoving = handleMovement(deltaTime);
    handleRotation(window);
    updateInvincibility(deltaTime);
    updateAnimation(deltaTime, isMoving);

    m_weapon.update(deltaTime, m_sprite.getPosition(), m_sprite.getRotation());
}

bool Player::handleMovement(sf::Time deltaTime)
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

    bool isMoving = (movement.x != 0.f || movement.y != 0.f);

    m_sprite.move(movement * deltaTime.asSeconds());

    return isMoving;
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

void Player::updateAnimation(sf::Time deltaTime, bool isMoving)
{
    Animation* targetAnimation = isMoving ? &m_walkAnimation : &m_idleAnimation;

    // Only reset when actually SWITCHING states — resetting every
    // frame would restart the animation from frame 0 constantly,
    // which looks like it's not animating at all.
    if (targetAnimation != m_currentAnimation)
    {
        m_currentAnimation = targetAnimation;
        m_currentAnimation->reset();
    }

    m_currentAnimation->update(deltaTime);
    m_sprite.setTextureRect(m_currentAnimation->getCurrentFrameRect());
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
    m_weapon.draw(window);
}

bool Player::shoot()
{
    return m_weapon.shoot();
}

void Player::reload()
{
    m_weapon.reload();
}

bool Player::isWeaponAutomatic() const
{
    return m_weapon.isAutomatic();
}

int Player::getAmmo() const
{
    return m_weapon.getCurrentAmmo();
}

int Player::getMagazineSize() const
{
    return m_weapon.getMagazineSize();
}

bool Player::isReloading() const
{
    return m_weapon.isReloading();
}

int Player::checkHit(const sf::FloatRect& targetBounds)
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

void Player::reset(sf::Vector2f spawnPosition)
{
    m_currentHealth = m_maxHealth;
    m_score = 0;
    m_isInvincible = false;
    m_invincibilityTimer = sf::Time::Zero;
    m_weapon.refillAmmo();
    setPosition(spawnPosition);
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

void Player::heal(int amount)
{
    m_currentHealth = std::min(m_currentHealth + amount, m_maxHealth);
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

void Player::refillAmmo()
{
    m_weapon.refillAmmo();
}

void Player::addScore(int amount)
{
    m_score += amount;
}

int Player::getScore() const
{
    return m_score;
}

void Player::setHealth(int health)
{
    m_currentHealth = std::clamp(health, 0, m_maxHealth);
}

void Player::setScore(int score)
{
    m_score = score;
}

WeaponType Player::getWeaponType() const
{
    return m_weapon.getType();
}

void Player::setWeaponType(WeaponType type)
{
    m_weapon.setType(type);
}