#pragma once

#include <SFML/Graphics.hpp>
#include "player/Player.h"
#include "enemy/EnemyManager.h"
#include "graphics/Camera.h"
#include "world/TileMap.h"

class Game
{
public:
    Game();

    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

private:
    sf::RenderWindow m_window;

    TileMap m_tileMap;
    sf::Vector2f m_worldSize;

    Player m_player;
    EnemyManager m_enemyManager;
    Camera m_camera;

    bool m_gameOver;

    sf::Font m_font;
    sf::Text m_gameOverText;
};