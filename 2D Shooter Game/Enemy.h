#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
private:
    sf::RectangleShape body;

    float speed;
    int health;

public:
    Enemy(sf::Vector2f position);

    void update(float deltaTime, sf::Vector2f playerPos);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    void takeDamage(int damage);
    bool isDead() const;
};