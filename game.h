#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <optional>

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "Obstacle.h"

class Game
{
private:
    sf::RenderWindow window;

    // Textures
    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture bulletTexture;
    sf::Texture towerTexture;
    sf::Texture backgroundTexture;

    // Music
    sf::Music music;

    // Background
    std::optional<sf::Sprite> background;

    // Player
    std::optional<Player> player;

    // Game Objects
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enemyBullets;
    std::vector<Obstacle> obstacles;

    // Game State
    int playerHP;
    int kills;

    bool gameOver;
    bool gameWon;

    // Constants
    const float playerSpeed = 300.f;
    const float enemySpeed = 120.f;

    // Timers
    sf::Clock clock;
    sf::Clock spawnClock;

    // Random
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> xDist;
    std::uniform_real_distribution<float> yDist;
    std::uniform_real_distribution<float> shootRandom;

    // Initialization
    void startWindow();
    void loadTextures();
    void loadMusic();
    void initializeGame();

    // Collision
    void createObstacles();

public:
    Game();
    ~Game();

    void run();
};