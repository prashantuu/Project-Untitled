#include "Player.h"
Player::Player()
{
    body.setSize(sf::Vector2f(50.f, 50.f));

    body.setFillColor(sf::Color::Green);

    body.setPosition(300.f, 300.f);

    speed = 300.f;
    maxhealth = 100;
    health = maxhealth;
    maxanmo = 30;
    anmo = maxanmo;
    // RED BACKGROUND BAR
    healthBarBack.setSize(sf::Vector2f(100.f, 10.f));
    healthBarBack.setFillColor(sf::Color::Red);

    // GREEN FRONT BAR
    healthBarFront.setSize(sf::Vector2f(100.f, 10.f));
    healthBarFront.setFillColor(sf::Color::Green);
    damageCooldown = 1.0f; // 1 second cooldown
    canTakeDamage = true;
}

void Player::update(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += speed * deltaTime;
    }
    body.move(movement);
    healthBarBack.setPosition(body.getPosition().x-25.f, body.getPosition().y - 15.f);
    healthBarFront.setPosition(body.getPosition().x-25.f, body.getPosition().y - 15.f);
    float healthPercent = (float)health / (float)maxhealth;
    healthBarFront.setSize(sf::Vector2f(100.f * healthPercent, 10.f));
    if (!canTakeDamage)
    {
        if (damageClock.getElapsedTime().asSeconds() >= damageCooldown)
        {
            canTakeDamage = true;
        }
    }
}

sf::FloatRect Player::getBounds()
{
    return body.getGlobalBounds();
}

void Player::takeDamage(int damage)
{
    if (!canTakeDamage)
        return;
    health -= damage;
    if (health < 0)
        health = 0;
    canTakeDamage = false;
    damageClock.restart();
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);
    window.draw(body);
}
sf::Vector2f Player::getPosition() const
{
    return body.getPosition();
}
int Player::getAmmo() const
{
    return anmo;
}

int Player::getHealth() const
{
    return health;
}

int Player::getMaxHealth() const
{
    return maxhealth;
}

void Player::useAmmo(int amount)
{
    anmo -= amount;
    if (anmo < 0)
        anmo = 0;
}
void Player::addHealth(int amount)
{
    health += amount;

    if (health > maxhealth)
        health = maxhealth;
}
void Player::addAmmo(int amount)
{
    anmo += amount;

    if (anmo > maxanmo)
        anmo = maxanmo;
}
