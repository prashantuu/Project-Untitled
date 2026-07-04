#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(sf::Vector2f size);

    // Camera only knows about positions — it has no idea
    // what a Player, Enemy, or anything else is.
    void follow(sf::Vector2f target);

    void apply(sf::RenderWindow& window);

private:
    sf::View m_view;
};