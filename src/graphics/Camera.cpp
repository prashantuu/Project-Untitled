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

void Camera::follow(sf::Vector2f target)
{
    // The camera's center can't go closer to an edge than half
    // the view size, or it would show space beyond the world.
    float halfWidth = m_viewSize.x / 2.f;
    float halfHeight = m_viewSize.y / 2.f;

    float clampedX = std::clamp(target.x, halfWidth, m_worldSize.x - halfWidth);
    float clampedY = std::clamp(target.y, halfHeight, m_worldSize.y - halfHeight);

    m_view.setCenter({clampedX, clampedY});
}

void Camera::apply(sf::RenderWindow& window)
{
    window.setView(m_view);
}