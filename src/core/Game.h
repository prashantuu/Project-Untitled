#pragma once

#include <SFML/Graphics.hpp>
#include "resources/ResourceManager.h"
#include "player/Player.h"
#include "enemy/EnemyManager.h"
#include "graphics/Camera.h"
#include "world/TileMap.h"
#include "ui/HUD.h"
#include "audio/AudioManager.h"
#include "gameplay/WaveManager.h"
#include "pickup/PickupManager.h"

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
    PickupManager m_pickupManager;

    bool m_gameOver;
    bool m_wasTriggerHeldLastFrame;

    // Score is a Game-level concept — coins have no meaning inside
    // Player, so PickupManager reports coin values upward and Game
    // is the one that decides they add up to a "score".
    int m_score;

    sf::Font& m_font;
    sf::Text m_gameOverText;
    HUD m_hud;
};