#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(sf::Vector2f viewSize);

    // The camera needs to know how big the world is so it knows
    // how far it's allowed to move before showing empty space.
    void setWorldBounds(sf::Vector2f worldSize);

    // deltaTime enables smooth following instead of an instant snap.
    void follow(sf::Vector2f target, sf::Time deltaTime);
    void apply(sf::RenderWindow& window);

private:
    sf::View m_view;
    sf::Vector2f m_viewSize;
    sf::Vector2f m_worldSize;
};