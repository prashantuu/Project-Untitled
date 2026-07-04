#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "enemy/Enemy.h"
#include "player/Player.h"

class EnemyManager
{
public:
    EnemyManager(sf::Vector2f worldSize);

    void update(sf::Time deltaTime, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);

    void spawnEnemy(sf::Vector2f position);

    void checkCollisions(Player& player);
    void checkPlayerCollision(Player& player);

private:
    void updateSpawning(sf::Time deltaTime);
    sf::Vector2f getRandomSpawnPosition();

private:
    std::vector<Enemy> m_enemies;

    sf::Vector2f m_worldSize;

    sf::Time m_spawnTimer;
    static const sf::Time SPAWN_INTERVAL;
    static const std::size_t MAX_ENEMIES;

    std::mt19937 m_randomEngine;
};