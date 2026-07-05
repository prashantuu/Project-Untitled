#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "bullet/Bullet.h"
#include "resource/ResourceManager.h"

enum class WeaponType
{
    Pistol,
    Shotgun,
    Rifle,
    SMG
};

class Weapon
{
public:
    Weapon(ResourceManager& resources, WeaponType type = WeaponType::Pistol);

    void update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation);
    void draw(sf::RenderWindow& window);

    // Returns true if a shot was actually fired. Safe to call every
    // frame — Weapon itself decides whether that results in a real
    // shot, based on fire rate, ammo, and reload state. This is what
    // lets the same call site support both semi-auto (called once
    // per click) and automatic (called every frame while held).
    bool shoot();
    void reload();
    void refillAmmo(); // instant free reload, e.g. from a pickup

    WeaponType getType() const;
    void setType(WeaponType type); // used by SaveManager on load
    bool isAutomatic() const;
    int getCurrentAmmo() const;
    int getMagazineSize() const;
    bool isReloading() const;

    // Returns damage dealt if a bullet hit targetBounds, 0 otherwise.
    int checkHit(const sf::FloatRect& targetBounds);

private:
    void applyStatsForType();
    void spawnBullet(float angleOffsetDegrees);

private:
    sf::Sprite m_sprite;
    sf::Texture& m_bulletTexture;
    std::vector<Bullet> m_bullets;

    WeaponType m_type;

    int m_damage;
    sf::Time m_fireRate;
    sf::Time m_fireCooldownTimer;

    int m_magazineSize;
    int m_currentAmmo;
    sf::Time m_reloadDuration;
    sf::Time m_reloadTimer;
    bool m_isReloading;

    bool m_isAutomatic;
    int m_pelletsPerShot;
    float m_spreadDegrees;

    std::mt19937 m_randomEngine;
};