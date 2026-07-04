#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "player/Player.h"
#include "enemy/Enemy.h"

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
    std::vector<Enemy> m_enemies;
};