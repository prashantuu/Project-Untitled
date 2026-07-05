#pragma once

#include <SFML/Graphics.hpp>
#include <random>

class Camera
{
public:
    Camera(sf::Vector2f viewSize);

    void setWorldBounds(sf::Vector2f worldSize);
    void follow(sf::Vector2f target, sf::Time deltaTime);
    void apply(sf::RenderWindow& window);

    // Triggers a temporary random shake that decays over duration —
    // e.g. call this when the player takes damage.
    void shake(float intensity, sf::Time duration);

    // 1.0 = normal. <1 = zoomed in, >1 = zoomed out.
    void setZoom(float zoomFactor);

private:
    sf::View m_view;
    sf::Vector2f m_baseViewSize; // original, unzoomed
    sf::Vector2f m_viewSize;     // current, after zoom applied
    sf::Vector2f m_worldSize;

    float m_shakeIntensity;
    sf::Time m_shakeDuration;
    sf::Time m_shakeTimer;

    std::mt19937 m_randomEngine;
};