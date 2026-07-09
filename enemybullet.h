#pragma once

#include <SFML/Graphics.hpp>

class EnemyBullet
{
public:
    sf::RectangleShape shape;
    sf::Vector2f direction;

    float speed;
    bool alive;

    EnemyBullet();

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);
};