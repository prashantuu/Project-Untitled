#include "weapon/Weapon.h"
#include <algorithm>
#include <cmath>
#include <ctime>

Weapon::Weapon(ResourceManager& resources, WeaponType type)
    : m_sprite(resources.getTexture("assets/sprites/weapon/gun.png"))
    , m_bulletTexture(resources.getTexture("assets/sprites/bullet/bullet.png"))
    , m_type(type)
    , m_damage(0)
    , m_fireRate(sf::Time::Zero)
    , m_fireCooldownTimer(sf::Time::Zero)
    , m_magazineSize(0)
    , m_currentAmmo(0)
    , m_reloadDuration(sf::Time::Zero)
    , m_reloadTimer(sf::Time::Zero)
    , m_isReloading(false)
    , m_isAutomatic(false)
    , m_pelletsPerShot(1)
    , m_spreadDegrees(0.f)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
    applyStatsForType();

    sf::Vector2u size = m_sprite.getTexture().getSize();
    m_sprite.setOrigin({0.f, size.y / 2.f});
}

void Weapon::applyStatsForType()
{
    // All weapon tuning lives here, keyed off the enum — adding a
    // 5th weapon later means adding one more case, not a new class.
    switch (m_type)
    {
        case WeaponType::Pistol:
            m_damage = 25;
            m_fireRate = sf::seconds(0.4f);
            m_magazineSize = 8;
            m_reloadDuration = sf::seconds(1.2f);
            m_isAutomatic = false;
            m_pelletsPerShot = 1;
            m_spreadDegrees = 0.f;
            break;

        case WeaponType::Shotgun:
            m_damage = 15;              // per pellet
            m_fireRate = sf::seconds(0.8f);
            m_magazineSize = 4;
            m_reloadDuration = sf::seconds(1.8f);
            m_isAutomatic = false;
            m_pelletsPerShot = 5;
            m_spreadDegrees = 25.f;
            break;

        case WeaponType::Rifle:
            m_damage = 20;
            m_fireRate = sf::seconds(0.15f);
            m_magazineSize = 25;
            m_reloadDuration = sf::seconds(1.6f);
            m_isAutomatic = true;
            m_pelletsPerShot = 1;
            m_spreadDegrees = 2.f;      // slight recoil spread
            break;

        case WeaponType::SMG:
            m_damage = 12;
            m_fireRate = sf::seconds(0.08f);
            m_magazineSize = 30;
            m_reloadDuration = sf::seconds(1.4f);
            m_isAutomatic = true;
            m_pelletsPerShot = 1;
            m_spreadDegrees = 4.f;      // more recoil, less accurate
            break;
    }

    m_currentAmmo = m_magazineSize;
}

void Weapon::update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation)
{
    m_sprite.setPosition(ownerPosition);
    m_sprite.setRotation(ownerRotation);

    m_fireCooldownTimer += deltaTime;

    if (m_isReloading)
    {
        m_reloadTimer += deltaTime;

        if (m_reloadTimer >= m_reloadDuration)
        {
            m_isReloading = false;
            m_currentAmmo = m_magazineSize;
            m_reloadTimer = sf::Time::Zero;
        }
    }

    for (auto& bullet : m_bullets)
        bullet.update(deltaTime);

    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& bullet) { return !bullet.isAlive(); }),
        m_bullets.end());
}

void Weapon::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);

    for (auto& bullet : m_bullets)
        bullet.draw(window);
}

bool Weapon::shoot()
{
    if (m_isReloading)
        return false;

    if (m_fireCooldownTimer < m_fireRate)
        return false;

    if (m_currentAmmo <= 0)
        return false; // TODO: could play a distinct "empty click" sound here

    m_fireCooldownTimer = sf::Time::Zero;
    m_currentAmmo--;

    std::uniform_real_distribution<float> spreadDist(-m_spreadDegrees / 2.f, m_spreadDegrees / 2.f);

    for (int i = 0; i < m_pelletsPerShot; ++i)
    {
        float offset = (m_spreadDegrees > 0.f) ? spreadDist(m_randomEngine) : 0.f;
        spawnBullet(offset);
    }

    return true;
}

void Weapon::spawnBullet(float angleOffsetDegrees)
{
    sf::Vector2u size = m_sprite.getTexture().getSize();

    sf::Vector2f spawnPosition = m_sprite.getTransform().transformPoint(
        { static_cast<float>(size.x), size.y / 2.f });

    sf::Angle finalAngle = m_sprite.getRotation() + sf::degrees(angleOffsetDegrees);

    m_bullets.emplace_back(m_bulletTexture, spawnPosition, finalAngle, m_damage);
}

void Weapon::reload()
{
    if (m_isReloading)
        return;

    if (m_currentAmmo == m_magazineSize)
        return; // already full, nothing to do

    m_isReloading = true;
    m_reloadTimer = sf::Time::Zero;
}

void Weapon::refillAmmo()
{
    m_currentAmmo = m_magazineSize;
    m_isReloading = false;
    m_reloadTimer = sf::Time::Zero;
}

WeaponType Weapon::getType() const
{
    return m_type;
}

void Weapon::setType(WeaponType type)
{
    m_type = type;
    applyStatsForType();
}

bool Weapon::isAutomatic() const
{
    return m_isAutomatic;
}

int Weapon::getCurrentAmmo() const
{
    return m_currentAmmo;
}

int Weapon::getMagazineSize() const
{
    return m_magazineSize;
}

bool Weapon::isReloading() const
{
    return m_isReloading;
}

int Weapon::checkHit(const sf::FloatRect& targetBounds)
{
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
    {
        if (it->getBounds().findIntersection(targetBounds))
        {
            int damage = it->getDamage();
            m_bullets.erase(it);
            return damage;
        }
    }

    return 0;
}