#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy/Enemy.h"
#include "player/Player.h"

class EnemyManager
{
public:
    EnemyManager();

    void update(sf::Time deltaTime, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);

    void spawnEnemy(sf::Vector2f position);

    // Checks all enemies against the player's bullets, destroying
    // any enemy that got hit.
    void checkCollisions(Player& player);

    // Checks if any enemy is touching the player. If so, it tells
    // the player to take damage — it never modifies health itself.
    void checkPlayerCollision(Player& player);

private:
    std::vector<Enemy> m_enemies;
};