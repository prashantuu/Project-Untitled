#pragma once

#include <SFML/Graphics.hpp>
#include "player/Player.h"
#include "enemy/EnemyManager.h"
#include "graphics/Camera.h"

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
    Player m_player;
    EnemyManager m_enemyManager;
    Camera m_camera;

    // Simple flag for now. We'll upgrade this to a proper
    // GameState enum once we have more than one non-playing state
    // (menu, pause, victory, etc.).
    bool m_gameOver;

    sf::Font m_font;
    sf::Text m_gameOverText;
};