#pragma once

#include <SFML/Graphics.hpp>

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
    sf::CircleShape   m_player;

    static const float PLAYER_SPEED;
};