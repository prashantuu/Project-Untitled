#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:

    sf::RectangleShape body;
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    float speed;
    int health;
    int maxhealth;
    int anmo;
    int maxanmo;
    sf::Clock damageClock;
    float damageCooldown;
    bool canTakeDamage;
public:

    Player();

    void update(float deltaTime);
    sf::FloatRect getBounds();
    void takeDamage(int damage);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    int getAmmo() const;
    int getHealth() const;
    int getMaxHealth() const;
    void useAmmo(int amount);
    void addHealth(int amount);
    void addAmmo(int amount);
};