#pragma once

#include <SFML/Graphics.hpp>
#include "resource/ResourceManager.h"
#include "player/Player.h"
#include "enemy/EnemyManager.h"
#include "graphics/Camera.h"
#include "world/TileMap.h"
#include "ui/HUD.h"
#include "audio/AudioManager.h"
#include "gameplay/WaveManager.h"

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
    // Declared first: everything below constructs itself by asking
    // this for resources, so it must exist before them.
    ResourceManager m_resources;

    sf::RenderWindow m_window;

    TileMap m_tileMap;
    sf::Vector2f m_worldSize;

    Player m_player;
    EnemyManager m_enemyManager;
    WaveManager m_waveManager;
    Camera m_camera;
    AudioManager m_audioManager;

    bool m_gameOver;

    sf::Font& m_font;
    sf::Text m_gameOverText;
    HUD m_hud;
};