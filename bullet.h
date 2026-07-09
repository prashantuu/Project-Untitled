#pragma once
#include <SFML/Graphics.hpp>
class Bullet
{
public:
    sf::Sprite shape;
    sf::Vector2f direction;
    float speed;
    bool alive;

   Bullet(const sf::Texture& texture);

void update(float deltaTime);
void draw(sf::RenderWindow& window);
};