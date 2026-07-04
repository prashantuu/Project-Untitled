#pragma once

#include <SFML/Graphics.hpp>
#include "weapon/Weapon.h"

class Player
{
public:
    Player();

    void update(sf::Time deltaTime, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void shoot();
    bool checkHit(const sf::FloatRect& targetBounds);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void setPosition(sf::Vector2f position);

    void constrainToWorld(sf::Vector2f worldSize);

    bool takeDamage(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    bool isAlive() const;

private:
    void handleMovement(sf::Time deltaTime);
    void handleRotation(const sf::RenderWindow& window);
    void updateInvincibility(sf::Time deltaTime);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    Weapon m_weapon;

    int m_currentHealth;
    int m_maxHealth;

    bool m_isInvincible;
    sf::Time m_invincibilityTimer;

    static const float SPEED;
    static const sf::Time INVINCIBILITY_DURATION;
};