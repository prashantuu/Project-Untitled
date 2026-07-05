#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "enemy/Enemy.h"
#include "player/Player.h"
#include "resource/ResourceManager.h"

class EnemyManager
{
public:
    EnemyManager(sf::Vector2f worldSize, ResourceManager& resources);

    void update(sf::Time deltaTime, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);

    // Mechanism only: EnemyManager knows HOW to spawn (position,
    // texture) but no longer decides WHEN or HOW MANY — that's
    // WaveManager's job now.
    void spawnEnemy(sf::Vector2f position);
    void spawnRandomEnemy();

    std::size_t checkCollisions(Player& player);
    bool checkPlayerCollision(Player& player);

    std::size_t getEnemyCount() const;

private:
    sf::Vector2f getRandomSpawnPosition();

private:
    sf::Texture& m_enemyTexture;

    std::vector<Enemy> m_enemies;

    sf::Vector2f m_worldSize;

    std::mt19937 m_randomEngine;
};