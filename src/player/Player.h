#pragma once

#include <SFML/Graphics.hpp>
#include "weapon/Weapon.h"
#include "resource/ResourceManager.h"
#include "graphics/Animation.h"

class Player
{
public:
    Player(ResourceManager& resources);

    void update(sf::Time deltaTime, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    // Returns true if a shot actually fired (not blocked by fire
    // rate, empty magazine, or reload) — Game uses this to decide
    // whether to play a gunshot sound.
    bool shoot();
    void reload();
    bool isWeaponAutomatic() const;
    int getAmmo() const;
    int getMagazineSize() const;
    bool isReloading() const;

    int checkHit(const sf::FloatRect& targetBounds);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void setPosition(sf::Vector2f position);

    void constrainToWorld(sf::Vector2f worldSize);

    bool takeDamage(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    bool isAlive() const;

private:
    bool handleMovement(sf::Time deltaTime);
    void handleRotation(const sf::RenderWindow& window);
    void updateInvincibility(sf::Time deltaTime);
    void updateAnimation(sf::Time deltaTime, bool isMoving);

private:
    sf::Sprite m_sprite;
    Weapon m_weapon;

    Animation m_idleAnimation;
    Animation m_walkAnimation;
    Animation* m_currentAnimation;

    int m_currentHealth;
    int m_maxHealth;

    bool m_isInvincible;
    sf::Time m_invincibilityTimer;

    static const float SPEED;
    static const sf::Time INVINCIBILITY_DURATION;
};