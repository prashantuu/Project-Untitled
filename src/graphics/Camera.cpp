#include "graphics/Camera.h"
#include <algorithm>

Camera::Camera(sf::Vector2f viewSize)
    : m_view(sf::FloatRect({0.f, 0.f}, viewSize))
    , m_viewSize(viewSize)
    , m_worldSize(viewSize) // default: world same size as view until told otherwise
{
}

void Camera::setWorldBounds(sf::Vector2f worldSize)
{
    m_worldSize = worldSize;
}

void Camera::follow(sf::Vector2f target, sf::Time deltaTime)
{
    sf::Vector2f currentCenter = m_view.getCenter();

    // Smoothly interpolate toward the target rather than snapping.
    // Higher SMOOTHING = snappier; lower = laggier/floatier feel.
    const float SMOOTHING = 6.f;
    float lerpFactor = std::min(SMOOTHING * deltaTime.asSeconds(), 1.f);

    sf::Vector2f smoothedCenter = currentCenter + (target - currentCenter) * lerpFactor;

    float halfWidth = m_viewSize.x / 2.f;
    float halfHeight = m_viewSize.y / 2.f;

    float clampedX = std::clamp(smoothedCenter.x, halfWidth, m_worldSize.x - halfWidth);
    float clampedY = std::clamp(smoothedCenter.y, halfHeight, m_worldSize.y - halfHeight);

    m_view.setCenter({clampedX, clampedY});
}

void Camera::apply(sf::RenderWindow& window)
{
    window.setView(m_view);
}