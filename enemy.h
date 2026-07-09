#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    sf::Sprite shape;

    int hp;
    bool alive;
    float shootTimer;

    Enemy(const sf::Texture& texture);

    void update(sf::Vector2f target, float speed, float deltaTime);

    void draw(sf::RenderWindow& window);
};