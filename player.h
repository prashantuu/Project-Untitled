#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::Sprite sprite;

    float speed;
    float angle;
    float walkTime;
bool moving;
sf::Vector2f basePosition;

public:
    Player(const sf::Texture& texture);

    void setPosition(sf::Vector2f position);

    void move(sf::Vector2f movement);

    void rotate(sf::Vector2f mousePos);

    sf::Sprite& getSprite();

    sf::Vector2f getPosition() const;

    sf::FloatRect getGlobalBounds() const;

    float getAngle() const;

    void draw(sf::RenderWindow& window);
};