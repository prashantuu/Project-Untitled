#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Pickup.h"
#include <vector>
using namespace sf;
class Game
{
private:

    sf::RenderWindow window;

    Player player;

    sf::Clock clock;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Pickup> pickups;
public:

    Game();

    void run();

    void processEvents();

    void update(float deltaTime);

    void render();
    sf::Font font;
    sf::Text ammotext;
};