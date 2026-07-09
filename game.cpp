#include "Game.h"
#include <cmath>
#include <algorithm>
#include <optional>
#include <string>
#include<iostream>
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
Game::Game()
    : gen(rd()),
      xDist(0.f, 900.f),
      yDist(0.f, 600.f),
      shootRandom(1.f, 3.f)
{
    startWindow();
    loadTextures();
    loadMusic();
    initializeGame();
 createObstacles();
}
Game::~Game()
{
}
void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    Bullet b(bulletTexture);

                    float muzzleForward = 95.f;
                    float muzzleSide = -6.f;

                    float rad = player->getAngle() * 3.14159265f / 180.f;

                    sf::Vector2f center = player->getPosition();

                    center.x += std::cos(rad) * muzzleForward
                              - std::sin(rad) * muzzleSide;

                    center.y += std::sin(rad) * muzzleForward
                              + std::cos(rad) * muzzleSide;

                    b.shape.setPosition(center);

                    sf::Vector2f mousePos =
                        window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    sf::Vector2f dir = mousePos - center;

                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                    if (len != 0)
                        dir /= len;

                    b.direction = dir;

                    float angle =
                        atan2(dir.y, dir.x) * 180.f / 3.14159265f;

                    b.shape.setRotation(sf::degrees(angle));

                    bullets.push_back(b);
                }
            }
        }
// PLAYER MOVE
sf::Vector2f movement(0.f, 0.f);

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    movement.y -= playerSpeed * deltaTime;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    movement.y += playerSpeed * deltaTime;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    movement.x -= playerSpeed * deltaTime;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    movement.x += playerSpeed * deltaTime;

sf::Vector2f oldPos = player->getPosition();

player->move(movement);

for (auto &wall : obstacles)
{
    if (player->getGlobalBounds().findIntersection(wall.getBounds()))
    {
        player->setPosition(oldPos);
        break;
    }
}
// Rotate player toward mouse
sf::Vector2f mousePos =
    window.mapPixelToCoords(sf::Mouse::getPosition(window));

player->rotate(mousePos);
// SPAWN ENEMIES
if (spawnClock.getElapsedTime().asSeconds() > 2.f)
{
Enemy e(enemyTexture);

    e.shape.setTexture(enemyTexture);

    int side = rand() % 4;

    if (side == 0)
        e.shape.setPosition({0.f, yDist(gen)});

    if (side == 1)
        e.shape.setPosition({900.f, yDist(gen)});

    if (side == 2)
        e.shape.setPosition({xDist(gen), 0.f});

    if (side == 3)
        e.shape.setPosition({xDist(gen), 600.f});

    e.shootTimer = shootRandom(gen);

    enemies.push_back(e);

    spawnClock.restart();
}
// ENEMY AI
for (auto& e : enemies)
{
    if (!e.alive)
        continue;

    e.update(player->getPosition(), enemySpeed, deltaTime);

    e.shootTimer -= deltaTime;

    if (e.shootTimer <= 0.f)
    {
        e.shootTimer = shootRandom(gen);

        EnemyBullet b;

        sf::Vector2f start = e.shape.getPosition();
        b.shape.setPosition(start);

        sf::Vector2f target = player->getPosition() - start;

        float len = std::sqrt(target.x * target.x + target.y * target.y);

        if (len != 0)
            target /= len;

        b.direction = target;

        enemyBullets.push_back(b);
    }
}
// PLAYER BULLETS
for (auto& b : bullets)
{
    if (!b.alive)
        continue;

    b.update(deltaTime);

    sf::Vector2f bp = b.shape.getPosition();
    sf::FloatRect bs = b.shape.getGlobalBounds();

    for (auto& e : enemies)
    {
        if (!e.alive)
            continue;

    sf::FloatRect eb = e.shape.getGlobalBounds();

bool collision =
    bp.x < eb.position.x + eb.size.x &&
    bp.x + bs.size.x > eb.position.x &&
    bp.y < eb.position.y + eb.size.y &&
    bp.y + bs.size.y > eb.position.y;

        if (collision)
        {
            b.alive = false;
            e.hp--;

            if (e.hp <= 0)
            {
                e.alive = false;
                e.shape.setPosition({-1000.f, -1000.f});
                kills++;
            }

            break;
        }
    }
}
// ENEMY BULLETS
for (auto& b : enemyBullets)
{
    if (!b.alive)
        continue;

    b.update(deltaTime);

    sf::Vector2f bp = b.shape.getPosition();
    sf::Vector2f bs = b.shape.getSize();

    // Player collision
    sf::FloatRect ps = player->getSprite().getGlobalBounds();
    sf::Vector2f pp = player->getPosition();

    if (bp.x < pp.x + ps.size.x &&
        bp.x + bs.x > pp.x &&
        bp.y < pp.y + ps.size.y &&
        bp.y + bs.y > pp.y)
    {
        b.alive = false;
        playerHP--;
    }

    
}
bullets.erase(
    std::remove_if(
        bullets.begin(),
        bullets.end(),
        [](Bullet& b)
        {
            return !b.alive;
        }),
    bullets.end());

enemyBullets.erase(
    std::remove_if(
        enemyBullets.begin(),
        enemyBullets.end(),
        [](EnemyBullet& b)
        {
            return !b.alive;
        }),
    enemyBullets.end());
    if (kills >= 20)
    gameWon = true;

if (playerHP <= 0)
    gameOver = true;
     // Update window title
window.setTitle(
    "Kills: " + std::to_string(kills) + "/20 | " +
    "Player HP: " + std::to_string(playerHP) + "/5 | " 
);

// DRAW
window.clear();
window.draw(*background);
player->draw(window);
for (auto& e : enemies)
{
    if (e.alive)
        e.draw(window);
}

for (auto& b : bullets)
{
    b.draw(window);
}

for (auto& b : enemyBullets)
{
    b.draw(window);
}
window.display();
    }
}
void Game::startWindow()
{
    window.create(
        sf::VideoMode({900, 600}),
        "Tower Defense Shooter"
    );

    window.setFramerateLimit(60);
}

void Game::loadTextures()
{
if (!playerTexture.loadFromFile("player.png"))
{
    throw std::runtime_error("Failed to load player.png");
}

player.emplace(playerTexture);

if (!enemyTexture.loadFromFile("enemy.png"))
{
    throw std::runtime_error("Failed to load enemy.png");
}
if (!bulletTexture.loadFromFile("bullet.png"))
{
    throw std::runtime_error("Failed to load bullet.png");
};

if (!backgroundTexture.loadFromFile("background.png"))
{
    throw std::runtime_error("Failed to load background.png");
}
background.emplace(backgroundTexture);

    background->setScale({
        900.f / backgroundTexture.getSize().x,
        600.f / backgroundTexture.getSize().y
    });
}

void Game::loadMusic()
{
    if (music.openFromFile("background.ogg.mp3"))
    {
        music.setLooping(true);
        music.setVolume(30.f);
        music.play();
    }
}
void Game::initializeGame()
{
    player->setPosition({430.f, 430.f});

    playerHP = 5;
    kills = 0;
    gameOver = false;
    gameWon = false;
}
void Game::createObstacles()
{
    obstacles.clear();

    // Left structure
    obstacles.emplace_back(sf::Vector2f(35,170), sf::Vector2f(140,250));

    // Left lower pillar
    obstacles.emplace_back(sf::Vector2f(170,250), sf::Vector2f(35,130));

    // Top middle left
    obstacles.emplace_back(sf::Vector2f(315,0), sf::Vector2f(40,135));

    // Top middle right
    obstacles.emplace_back(sf::Vector2f(550,0), sf::Vector2f(40,135));

    // Right lower pillar
    obstacles.emplace_back(sf::Vector2f(610,250), sf::Vector2f(35,130));

    // Right structure
    obstacles.emplace_back(sf::Vector2f(710,170), sf::Vector2f(155,250));

    // Right metal platform
    obstacles.emplace_back(sf::Vector2f(760,170), sf::Vector2f(95,90));
}
