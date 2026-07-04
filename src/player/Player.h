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

    // Clamps the player's position inside the given world size.
    // Player doesn't know or care what "the world" means beyond
    // these numbers — that's Game's decision to make.
    void constrainToWorld(sf::Vector2f worldSize);

    void takeDamage(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    bool isAlive() const;

private:
    void handleMovement(sf::Time deltaTime);
    void handleRotation(const sf::RenderWindow& window);
    void updateInvincibility(sf::Time deltaTime);

private:
    sf::RectangleShape m_shape;
    Weapon m_weapon;

    int m_currentHealth;
    int m_maxHealth;

    // Invincibility is entirely Player's own concern — nobody
    // outside this class needs to know it exists.
    bool m_isInvincible;
    sf::Time m_invincibilityTimer;

    static const float SPEED;
    static const sf::Time INVINCIBILITY_DURATION;
};