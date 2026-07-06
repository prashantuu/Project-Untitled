#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "enemy/Enemy.h"
#include "player/Player.h"
#include "bullet/Bullet.h"
#include "enemy/Boss.h"
#include "resources/ResourceManager.h"

class EnemyManager
{
public:
    EnemyManager(sf::Vector2f worldSize, ResourceManager& resources);

    void update(sf::Time deltaTime, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);

    void spawnEnemy(sf::Vector2f position, EnemyType type);
    void spawnRandomEnemy();

    // Player's bullets vs enemies — now applies damage instead of
    // an automatic one-hit kill, since Heavy enemies need to survive
    // multiple hits.
    // Returns how many enemies were destroyed this frame.
std::vector<sf::Vector2f> checkCollisions(Player& player);

    // Enemy bodies touching the player (melee).
    bool checkPlayerCollision(Player& player);

    // Shooter enemies' bullets hitting the player (ranged).
    bool checkEnemyBulletHits(Player& player);

    std::size_t getEnemyCount() const;
    void reset();

    void spawnBoss(sf::Vector2f position);
    bool hasBoss() const;

private:
    sf::Vector2f getRandomSpawnPosition();
    EnemyType getRandomEnemyType();
    sf::Texture& getTextureForType(EnemyType type);

private:
    static const int ENEMY_BULLET_DAMAGE;
    static const int MELEE_DAMAGE;

    sf::Texture& m_fastTexture;
    sf::Texture& m_heavyTexture;
    sf::Texture& m_shooterTexture;

    // Shared with the player's own bullets in spirit (same Bullet
    // class), but this is a completely separate pool — enemy
    // bullets shouldn't be checked against enemies, and player
    // bullets shouldn't be checked against the player.
    sf::Texture& m_bossTexture;
    std::optional<Boss> m_boss;

    sf::Texture& m_enemyBulletTexture;
    std::vector<Bullet> m_enemyBullets;

    std::vector<Enemy> m_enemies;

    sf::Vector2f m_worldSize;

    std::mt19937 m_randomEngine;
};