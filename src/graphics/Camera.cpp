#include "graphics/Camera.h"

Camera::Camera(sf::Vector2f size)
    : m_view(sf::FloatRect({0.f, 0.f}, size))
{
}

void Camera::follow(sf::Vector2f target)
{
    // Center the view on whatever position it's told to follow.
    // The Camera doesn't know or care whose position this is.
    m_view.setCenter(target);
}

void Camera::apply(sf::RenderWindow& window)
{
    window.setView(m_view);
}