#pragma once

#include <SFML/Graphics.hpp>
#include "resources/ResourceManager.h"
#include "player/Player.h"
#include "enemy/EnemyManager.h"
#include "graphics/Camera.h"
#include "world/TileMap.h"
#include "ui/HUD.h"
#include "ui/Menu.h"
#include "audio/AudioManager.h"
#include "gameplay/WaveManager.h"
#include "pickup/PickupManager.h"
#include "save/SaveManager.h"
#include "graphics/ParticleSystem.h"

enum class GameState
{
    MainMenu,
    Playing,
    Paused,
    GameOver
};

class Game
{
public:
    Game();

    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void restartGame();

private:
    ResourceManager m_resources;

    sf::RenderWindow m_window;

    TileMap m_tileMap;
    sf::Vector2f m_worldSize;

    Player m_player;
    EnemyManager m_enemyManager;
    WaveManager m_waveManager;
    PickupManager m_pickupManager;
    Camera m_camera;
    ParticleSystem m_particleSystem;
    AudioManager m_audioManager;

    GameState m_state;
    bool m_wasTriggerHeldLastFrame;

    sf::Font& m_font;
    sf::Text m_gameOverText;
    HUD m_hud;

    Menu m_mainMenu;
    Menu m_pauseMenu;
};