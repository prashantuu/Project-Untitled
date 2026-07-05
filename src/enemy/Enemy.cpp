#include "enemy/Enemy.h"
#include <cmath>
#include <ctime>

Enemy::Enemy(sf::Texture& texture, sf::Vector2f spawnPosition, EnemyType type)
    : m_sprite(texture)
    , m_walkAnimation({32, 32}, 4, 0, sf::seconds(0.15f), true)
    , m_type(type)
    , m_behaviorState(EnemyBehaviorState::Patrol)
    , m_speed(0.f)
    , m_health(0)
    , m_maxHealth(0)
    , m_attackRange(0.f)
    , m_shootCooldown(sf::Time::Zero)
    , m_shootTimer(sf::Time::Zero)
    , m_spawnPosition(spawnPosition)
    , m_patrolTarget(spawnPosition)
    , m_lastKnownPlayerPosition(spawnPosition)
    , m_detectionRadius(250.f)
    , m_searchTimer(sf::Time::Zero)
    , m_searchDuration(sf::seconds(3.f))
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)) + reinterpret_cast<uintptr_t>(this))
{
    applyStatsForType();

    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
    m_sprite.setOrigin({16.f, 16.f});
    m_sprite.setPosition(spawnPosition);

    m_patrolTarget = pickNewPatrolTarget();
}

void Enemy::applyStatsForType()
{
    switch (m_type)
    {
        case EnemyType::Fast:
            m_speed = 160.f;
            m_maxHealth = 25;
            break;
        case EnemyType::Heavy:
            m_speed = 60.f;
            m_maxHealth = 100;
            break;
        case EnemyType::Shooter:
            m_speed = 90.f;
            m_maxHealth = 50;
            m_attackRange = 300.f;
            m_shootCooldown = sf::seconds(1.5f);
            break;
    }
    m_health = m_maxHealth;
}

sf::Vector2f Enemy::pickNewPatrolTarget()
{
    std::uniform_real_distribution<float> offsetDist(-150.f, 150.f);
    return { m_spawnPosition.x + offsetDist(m_randomEngine), m_spawnPosition.y + offsetDist(m_randomEngine) };
}

float Enemy::distanceTo(sf::Vector2f point) const
{
    sf::Vector2f diff = point - m_sprite.getPosition();
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void Enemy::update(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    updateBehaviorState(deltaTime, targetPosition);
    handleMovement(deltaTime, targetPosition);

    if (m_type == EnemyType::Shooter && m_behaviorState == EnemyBehaviorState::Attack)
    {
        m_shootTimer += deltaTime;
    }

    m_walkAnimation.update(deltaTime);
    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
}

void Enemy::updateBehaviorState(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    float distanceToPlayer = distanceTo(targetPosition);
    bool playerDetected = distanceToPlayer <= m_detectionRadius;

    if (playerDetected)
    {
        m_lastKnownPlayerPosition = targetPosition;
        m_searchTimer = sf::Time::Zero;

        if (m_type == EnemyType::Shooter && distanceToPlayer <= m_attackRange)
        {
            m_behaviorState = EnemyBehaviorState::Attack;
        }
        else
        {
            m_behaviorState = EnemyBehaviorState::Chase;
        }
        return;
    }

    // Player not detected — behavior depends on what we were doing.
    switch (m_behaviorState)
    {
        case EnemyBehaviorState::Chase:
        case EnemyBehaviorState::Attack:
            // Just lost track of the player — go check their last
            // known position instead of instantly giving up.
            m_behaviorState = EnemyBehaviorState::Search;
            m_searchTimer = sf::Time::Zero;
            break;

        case EnemyBehaviorState::Search:
            m_searchTimer += deltaTime;
            if (m_searchTimer >= m_searchDuration || distanceTo(m_lastKnownPlayerPosition) < 20.f)
            {
                m_behaviorState = EnemyBehaviorState::Return;
            }
            break;

        case EnemyBehaviorState::Return:
            if (distanceTo(m_spawnPosition) < 20.f)
            {
                m_behaviorState = EnemyBehaviorState::Patrol;
            }
            break;

        case EnemyBehaviorState::Patrol:
            // stays Patrol until player is detected
            break;
    }
}

void Enemy::moveToward(sf::Vector2f destination, sf::Time deltaTime)
{
    sf::Vector2f direction = destination - m_sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.f)
        direction /= length;

    m_sprite.move(direction * m_speed * deltaTime.asSeconds());
}

void Enemy::handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    switch (m_behaviorState)
    {
        case EnemyBehaviorState::Patrol:
            if (distanceTo(m_patrolTarget) < 15.f)
            {
                m_patrolTarget = pickNewPatrolTarget();
            }
            // Patrol at half speed — calmer than chasing.
            moveToward(m_patrolTarget, sf::seconds(deltaTime.asSeconds() * 0.5f));
            break;

        case EnemyBehaviorState::Chase:
            moveToward(targetPosition, deltaTime);
            break;

        case EnemyBehaviorState::Attack:
            // Shooter holds its ground once in range — no movement.
            break;

        case EnemyBehaviorState::Search:
            moveToward(m_lastKnownPlayerPosition, deltaTime);
            break;

        case EnemyBehaviorState::Return:
            moveToward(m_spawnPosition, deltaTime);
            break;
    }
}

bool Enemy::tryShoot(sf::Vector2f targetPosition)
{
    if (m_behaviorState != EnemyBehaviorState::Attack)
        return false;

    if (m_shootTimer < m_shootCooldown)
        return false;

    m_shootTimer = sf::Time::Zero;
    return true;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const
{
    return m_sprite.getPosition();
}

bool Enemy::takeDamage(int amount)
{
    m_health -= amount;
    return m_health <= 0;
}